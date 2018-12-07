#!/bin/bash
while true
do

  ioff_chan=0

  for file in HVSiPM_* ; do 
    if [ -f "$file" ]; then 
#        echo "$file" 
	while IFS='' read -r line || [[ -n "$line" ]]; do
	    #    echo "Text read from file: $line"
	    diff=$(awk '{split($0, a, ";"); diff= a[5]-a[4]; print diff} ' <<< $line )
	    mon=$(awk '{split($0, a, ";"); print a[5]}' <<< $line)
	    status=$(awk '{split($0, a, ";"); print a[9]}' <<< $line)
	    zero=0
	    #    diff=$(( ${a[5]}-${a[4]} ))
	    string="OK"
	    value=50.
	    if (( $(echo "$mon < $value" |bc -l) )); then
	      string="not OK"
	      ioff_chan=$((ioff_chan + 1))
	    fi	    
	    if (( $(echo "$status > $zero" | bc -l) )); then
		string="not OK"
		ioff_chan=$((ioff_chan + 1))
	    fi	    
#	    echo " hvmon=$mon  diff = $diff status=$status set=$string "
	done < <(tail -n +2 $file) 
   fi 
#   echo " ioff_chan= $ioff_chan "

  done

#  echo " total ioff_chan= $ioff_chan "
  echo "$ioff_chan" > ../nchan_summ_alarm_hvsipm.txt

  sleep 30
  
done
