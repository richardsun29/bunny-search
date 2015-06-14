#!/bin/bash

# Array of queries to search
declare -a queries=('bunny' 'rabbit' 'cute bunny' 'bunny gif')
#declare -a queries=('bunny')

# Number of items to search for (rounds now to a multiple of 8)
nitems=100


# Command line arguments
CACHE=true
eval set -- $(getopt -o f -l force-refresh -- "$@")
while [ $# -gt 0 ]; do
	case "$1" in
		-f | --force-refresh) CACHE=false;;
		(--) shift; break;;
		(-*) echo "$0: unrecognized option $1" 1>&2; exit 1;;
		(*) break;;
	esac
	shift
done


# Replace spaces with URL escape characters
for ((i = 0; i < ${#queries[@]}; i++)); do
	queries["$i"]=$(echo "${queries[$i]}" | sed -e 's/ /%20/g')
done

# add for other escape sequences
#-e 's/"/%22/g'
#-e 's/#/%23/g'
#-e 's/%/%25/g'

#echo ${queries[@]}

#curl -s "http://ajax.googleapis.com/ajax/services/search/images?q=cute%20bunny&v=1.0&start=0&rsz=8" | sed 's/,/\n/g' | grep unescapedUrl | awk -F'"' '{print $4}'

# Get image URLs
for query in ${queries[@]}; do
	printf "Retrieving search results for %s..." $query
	i=0

	# Cached results
	if [ $CACHE == true -a -f queries/"$query".txt ]; then
		printf "(cached)\n"
		continue
	fi

	urls=""
	for ((; i < nitems; i+=8)); do
		# Loading percent
		percent=$(bc <<< "scale = 3; $i/$nitems*100" | awk -F. '{print $1}')
		printf "%.0f%%" $percent

		urls+=$(curl -s "http://ajax.googleapis.com/ajax/services/search/images?q=$query&v=1.0&start=$i&rsz=8" | 
				sed 's/,/\n/g' | grep unescapedUrl | awk -F'"' '{print $4}')
		urls+=" "

#		[ $percent -ne 0 ] && printf "\b\b"
#		[ $percent -ge 10 ] && printf "\b"
		if [ $percent -lt 10 ]; then
			printf "\b\b"
		else
			printf "\b\b\b"
		fi
	done

	echo "$urls" | sed 's/ /\n/g' > queries/"$query".txt
	printf "100%%\n"
done


