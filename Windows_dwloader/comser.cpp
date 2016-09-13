//////////////////////////////////////////////////////////////////////
//
//	IMPLEMENTAZIONE DELLA CLASSE PER LA GESTIONE VERA E PROPRIA DELLA PORTA
//	SERIALE .QUESTA CLASSE SI OCCUPA DI IMPOSTARE LE RICHIESTE AL
//	PLC ,DI SPEDIRLE DI RICEVERLE E DECODIFICARLE
//	DEFINITA COME TIPO OGGETTO E' COMPLETAMENTE VIRTUALE
//
//		by Coppi Angelo
//		Coppi & Pinzi s.d.f 53021 Abbadia San Salvatore
//		S I E N A
//
//////////////////////////////////////////////////////////////////////




#include "StdAfx.h"

#include "ComSer.h"

#define CC_START    255
#define READ_MEMORY			59
#define WRITE_MEMORY		60
#define READ_ONE_MEMORY     28
#define WRITE_ONE_MEMORY    29

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


DCB CComSer::dcb;

IMPLEMENT_DYNCREATE(CComSer, CObject)
/////////////////////////////////////////////////////////////////////////////
// CComSer::CComSer - Constructor

CComSer::CComSer()
{
// INIZIAZIONE COSTRUTTORE OGGETTO COMSER
memset(ComBufInp,0,IO_SIZE * sizeof(unsigned char));// RESET BUFFER TEMPORANEO DI INGRESSO
memset(ComBufOut,0, IO_SIZE * sizeof(unsigned char));// RESET BUFFER TEMPORANEO DI USCITA
m_Com=INVALID_HANDLE_VALUE;
}


/////////////////////////////////////////////////////////////////////////////
// CComSer::CComSer - Destructor

CComSer::~CComSer()
{
    CloseComm();
}





BOOL CComSer::OpenComm(int COM,int VEL,int RxSize,int TxSize)
{
    CString com;
com.Format("COM%d",COM);
// APRO LA PORTA DI COMUNICAZIONE 
if (m_Com != INVALID_HANDLE_VALUE)
    CloseHandle(m_Com);
m_Com=CreateFile((LPCSTR)com,
        GENERIC_READ | GENERIC_WRITE,
        0,      //  must be opened with exclusive-access
        NULL,   //  default security attributes
        OPEN_EXISTING, //  must use OPEN_EXISTING
        0,      //  not overlapped I/O
        NULL); //  hTemplate must be NULL for comm devices
if(m_Com==INVALID_HANDLE_VALUE)
    {
    return FALSE;
    }
else 
    {
        int err;
    // abilito evento rxchar
    err=SetCommMask( m_Com, EV_RXCHAR ) ;
    // buffer queque 
    err=SetupComm( m_Com, RxSize, TxSize ) ;
    // reset dati preesistenti
    err = PurgeComm( m_Com, PURGE_TXABORT | PURGE_RXABORT |
                      PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
    // ritorno immediato da operazioni di read
    COMMTIMEOUTS CommTimeOuts;
    CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
    CommTimeOuts.ReadTotalTimeoutConstant = 1000 ;
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0 ;
    CommTimeOuts.WriteTotalTimeoutConstant = 1000 ;
    err=SetCommTimeouts( m_Com, &CommTimeOuts ) ;
    // Legge i dati di default ed inizialicca il DCB locale
    memset(&dcb, 0, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB);
    /*
    if (GetCommState(m_Com, &dcb) == FALSE) {
        dcb.fBinary = FALSE;
        dcb.fParity = FALSE;
        dcb.fOutxCtsFlow = FALSE;
        dcb.fOutxDsrFlow = FALSE;
        dcb.fDtrControl = DTR_CONTROL_DISABLE;
        dcb.fDsrSensitivity = FALSE;
        dcb.fTXContinueOnXoff = FALSE;
        dcb.fOutX = FALSE;
        dcb.fInX = FALSE;
        dcb.fErrorChar = FALSE;
        dcb.fNull = FALSE;
        dcb.fRtsControl = RTS_CONTROL_DISABLE;
        dcb.fAbortOnError = FALSE;
        dcb.wReserved = 0;
        dcb.XonLim = 0;
        dcb.XoffLim = 0;
        dcb.XonChar = 0;
        dcb.XoffChar = 0;
        dcb.ErrorChar = 0x00;
        dcb.EofChar = 0;
        dcb.EvtChar = 0;
    }
    //dcb.BaudRate = VEL;
    dcb.ByteSize = 8;
    dcb.Parity = 0;
    dcb.StopBits = 1;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fDtrControl = DTR_CONTROL_DISABLE;
    dcb.fDsrSensitivity = FALSE;
    dcb.fRtsControl = RTS_CONTROL_DISABLE;
    */
    
    com.Format("baud=%d parity=N data=8 stop=1 ",VEL);
    err=BuildCommDCB((LPCSTR)com,&dcb);
    // IMPOSTAZIONI PARTICOLARI NULL MODEM
    switch (VEL)
    {
    case 600:dcb.BaudRate = CBR_600; break;
    case 1200:dcb.BaudRate = CBR_1200; break;
    case 2400:dcb.BaudRate = CBR_2400; break;
    case 4800:dcb.BaudRate = CBR_4800; break;
    case 9600:dcb.BaudRate = CBR_9600; break;
    case 19200:dcb.BaudRate = CBR_19200; break;
    case 38400:dcb.BaudRate = CBR_38400; break;
    case 57600:dcb.BaudRate = CBR_57600; break;
    case 115200:dcb.BaudRate = CBR_115200; break;
    }
    dcb.fBinary = TRUE;
    dcb.fParity = FALSE;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
    dcb.fNull = FALSE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
    dcb.fErrorChar = FALSE;
    dcb.fNull = FALSE;
    dcb.fRtsControl = FALSE;
    dcb.fDtrControl = FALSE;

    err=SetCommState(m_Com,&dcb);
    if (!err)
    {
        err = ::GetLastError();
    }
    return TRUE;
    }
}



int  CComSer::TxComm(FILE *file)
{
    int stop = 0;
    int result = -1;
    if (file != NULL)
    {
        unsigned char tmp;
        memset(ComBufOut, 0, sizeof(ComBufOut));
        int pos = 0;
        do 
        {
            if (1 != fread(&tmp, 1, 1, file))
            {
                stop = 1;
                break;
            }
        } while (tmp < 32); // Skip unconsistent chars
        ComBufOut[pos++] = tmp;
        do
        {
            if (1 != fread(&tmp, 1, 1, file))
            {
                stop = 1;
                break;
            }
            if (tmp < 32)break;
            ComBufOut[pos++] = tmp;
            if (pos > IO_SIZE)break;
        } while (tmp > 32); // store line
        PosRd = 0;
        if(!stop)
            result=Trasmetti(pos);

    }
    return  result;
}




int CComSer::Trasmetti(size_t pos)
{
//SCRITTURA SU PORTA SERIALE+GESTIONE ERRORI
BOOL        flagStato;		// risultato ritorno da scrittura
DWORD       dwScrittura ;	// numero di byte scritti se errore
DWORD   	dwErrore;		// errore eventuale
COMSTAT     Stato;			// riceve lo stato della porta per reset errori
OVERLAPPED  ovr = {};		// OVERLAPPED I/O
//SCRIVO SU SERIALE
memset(&ovr, 0, sizeof(ovr));
int result = -1;
flagStato = ::WriteFile( m_Com,ComBufOut,pos,&dwScrittura, &ovr) ;
int error = ::GetLastError();
if (!flagStato)
{
    // GESTIONE ERRORI IN SCRITTURA
    if (error == ERROR_IO_PENDING)
    {

        while (!GetOverlappedResult(m_Com, &ovr, &dwScrittura, TRUE))
        {
            dwErrore = ::GetLastError();
            if (dwErrore == ERROR_IO_INCOMPLETE)
                // per forza non ha finito la scrittura
                continue;
            else
            {
                // memorizzo errore
                // reset porta di errore
                ClearCommError(m_Com, &dwErrore, &Stato);
                break;
            }
        }
        }
    else
    {
        // ALTRO TIPO DI ERRORE
        ClearCommError(m_Com, &dwErrore, &Stato);

    }
}
else
{
    result = 0;
    PosTx = pos;
}
return result;
}



DWORD CComSer::Ricevi(void)
{

BOOL       Stato;
COMSTAT    ComStat ;
DWORD      dwErrore;
DWORD      dwLen;
OVERLAPPED owr = {};
memset(&owr, 0, sizeof(owr));
//CI SONO CARATTERI DA RICEVERE
ClearCommError( m_Com, &dwErrore, &ComStat ) ;
dwLen = ComStat.cbInQue ;
// CI SONO CARATTERI
if (dwLen > 0)
    {
        Stato = ReadFile( m_Com, &ComBufInp[PosRd],dwLen, &dwLen,&owr) ;
        PosRd+=(int)dwLen;		
        if (!Stato)
        {
            // ERROR
            if (GetLastError() == ERROR_IO_PENDING)
            {
                while(!GetOverlappedResult( m_Com, 
                    &owr, &dwLen, TRUE ))
                {
                    dwErrore = GetLastError();
                    if(dwErrore == ERROR_IO_INCOMPLETE)
                        // NORMALE
                        continue;
                    else
                        {
                            ClearCommError( m_Com, &dwErrore, &ComStat ) ;
                            break;
                        }
                                        
                }
                    
            }
        else
        {						 	
            // ALTRO TIPO DI ERRORE
            ClearCommError( m_Com, &dwErrore, &ComStat ) ;
            
        }
   
      }
    }
   return ( dwLen ) ;
}

int CComSer::RxComm(void)
{
    int result = -1;
// RICEVO QUALCOSA
    if (Ricevi() > 0)
    {
        if (PosRd > 0)
        {
            if (PosTx >= PosRd)
            {
                if (memcmp(ComBufOut, ComBufInp, PosTx) == 0)
                {
                NoComm=result = 0;
                }
            }
        }
    }
    else
    {
        NoComm++;
        if (NoComm > 10)
        {
            result = -1000;
        }
    }
return result;
}






void CComSer::CloseComm(void)
{
if(m_Com!=INVALID_HANDLE_VALUE)
    {
    // CHIAVE VALIDA LA PORTA e' APERTA
    //RIPULISCO LA PORTA DI COMUNICAZIONE
    PurgeComm( m_Com, PURGE_TXABORT | PURGE_RXABORT |
                      PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
    CloseHandle(m_Com);	
    NoComm=0;			// MASSIMO NUMERO DI MANCATE RISPOSTE DA SERIALE DOPO IL QUALE VIENE REIMPOSTATA UNA RICH.SERIALE	
    PosRd=0;
    PosTx=0;
    memset(ComBufInp, 0, sizeof(ComBufInp));
    memset(ComBufOut, 0, sizeof(ComBufOut));
    m_Com=INVALID_HANDLE_VALUE;
    }
}







