

#Colors
COLOR_WHITE=" -e \E[1;37m"
COLOR_GRAY_LIGHT=" -e \E[0;37m"
COLOR_GRAY_DARK=" -e \E[1;30m"
COLOR_BLUE=" -e \E[0;34m"
COLOR_BLUE_LIGHT=" -e \E[1;34m"
COLOR_GREEN=" -e \E[0;32m"
COLOR_GREEN_LIGHT=" -e \E[1;32m"
COLOR_CYAN=" -e \E[0;36m"
COLOR_CYAN_LIGHT=" -e \E[1;36m"
COLOR_RED=" -e \E[0;31m"
COLOR_RED_LIGHT=" -e \E[1;31m"
COLOR_PURPLE=" -e \E[0;35m"
COLOR_PURPLE_LIGHT=" -e \E[1;35m"
COLOR_BROWN=" -e \E[0;33m"
COLOR_YELLOW=" -e \E[1;33m"
COLOR_BLACK=" -e \E[0;30m"
REPLACE=" -e \E[0m"

#$1 color 
#$2 message
function echoC {
echo -e $(echo $1)"--->     " $2 $(echo $REPLACE) $3 $4 $5 $6
}


function clean_all_projects(){
CAOS_PRJ=$(ls | grep caos_)
for V in $CAOS_PRJ ; do
	make --silent -C $V/os distclean > /dev/null 
	make --silent -C $V/os_test distclean > /dev/null 
	DU=$(du -s -h $V)
	echoC  "$COLOR_GREEN_LIGHT"  "clean project : "  "size  $DU" 
done
}


EXT=$( date -R | awk '{print"_"$2"_"$3"_"$4"_"$5}' | sed -e 's/:/_/g')
if [ -d "./caos" ]; then 
	NDIR="caos$EXT"
	echoC "$COLOR_GREEN_LIGHT" "Move caos to $NDIR"
	mv caos "$NDIR"
	if [ $? -ne 0 ]; then
		echoC "$COLOR_RED_LIGHT" "ERROR : cannot move caos to $NDIR"
		exit 1
	fi
	sync
fi	
echoC "$COLOR_GREEN_LIGHT" "clone caos from GitHub...."
git clone -q https://github.com/ataima/caos.git caos 
clean_all_projects
cd caos 
make --silent -C os distclean all > /dev/null 
make --silent -C os_test distclean all > /dev/null
echoC "$COLOR_RED" "current status test :"
make -C os_test test | grep SUCCESS
echoC "$COLOR_YELLOW" "...Have a nice work ....bye..."	


