#!/bin/bash
COUNT=64
BLOCKSIZE=512
rm -f mount/* 
function do_small(){
for (( i=0;i<30;i++ ));do 
	echo frag mount_A/${i}
	if [[ 0 -eq $(( i % 2 )) ]]; then	
		./frag mount/A_${i} $(( BLOCKSIZE*1024 )) A $i
	else 
		./frag mount/B_${i} $(( BLOCKSIZE*1024 )) B $i
	fi
done
}

do_small 
ls -sh mount
df -h mount
echo "Removing B files"
rm -f mount/tstB* -f
./frag mount/C_0 $((COUNT*BLOCKSIZE*1024/2 )) C 1
ls -sh mount
df -h mount
