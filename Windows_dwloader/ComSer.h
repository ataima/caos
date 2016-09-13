//////////////////////////////////////////////////////////////////////
//
//	DEFINIONE DELLA CLASSE PER LA GESTIONE VERA E PROPRIA DELLA PORTA
//	SERIALE .QUESTA CLASSE SI OCCUPA DI IMPOSTARE LE RICHIESTE AL
//	PLC ,DI SPEDIRLE DI RICEVERLE E DECODIFICARLE
//	DEFINITA COME TIPO OGGETTO E' COMPLETAMENTE VIRTUALE
//
//		by Coppi Angelo
//		Coppi & Pinzi s.d.f 53021 Abbadia san Salvatore
//		S I E N A
//
//////////////////////////////////////////////////////////////////////

#define IO_SIZE 4096

class CComSer: public CObject
{
DECLARE_DYNCREATE(CComSer)

public :
    CComSer();
    ~CComSer();
    virtual BOOL OpenComm(int,int,int,int);			
    virtual void CloseComm(void);
    virtual int RxComm();
    virtual int TxComm(FILE *file);
    inline int Ritrasmetti(void) { PosRd = 0; return  Trasmetti(PosTx); }

    // definizioni per comunicazione con plc sono publiche perche
    // la classe che chiama questa deve poter accedere al tipo di richiesta
private:
    HANDLE m_Com;		// COPIA ALLA CHIAVE DEL FILE APERTO
    int NoComm;			// MASSIMO NUMERO DI MANCATE RISPOSTE DA SERIALE DOPO IL QUALE VIENE REIMPOSTATA UNA RICH.SERIALE	
    static DCB dcb;			// STRUTTURA PARAMETRI DI COMUNICAZIONE
    size_t PosRd;
    size_t PosTx;
    //VARIABILI DI STATO PLC
    unsigned char ComBufInp[IO_SIZE];	// BUFFER TEMPORANEO DI LETTURA
    unsigned char ComBufOut[IO_SIZE];	// BUFFER TEMPORANEO DI SCRITTURA

protected:
     int Trasmetti(size_t pos);
     DWORD  Ricevi(void);

};

