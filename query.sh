#!/bin/bash

# Array of queries to search
declare -a queries=('bunny' 'rabbit' 'cute bunny')

# Number of items to search for
nitems=100

# Replace spaces with URL escape characters
for ((i = 0; i < ${#queries[@]}; i++)); do
	queries["$i"]=$(echo "${queries[$i]}" | sed -e 's/ /%20/g')
done

# add for other escape sequences
#-e 's/"/%22/g' \
#-e 's/#/%23/g' \
#-e 's/%/%25/g' )

#echo ${queries[@]}

#curl -s "http://ajax.googleapis.com/ajax/services/search/images?q=cute%20bunny&v=1.0&start=0&rsz=8" | sed 's/,/\n/g' | grep unescapedUrl | awk -F'"' '{print $4}'

# Get image URLs
for query in ${queries[@]}; do
	urls=""
	for ((i = 0; i < nitems; i+=8)); do
		urls+=$(curl -s "http://ajax.googleapis.com/ajax/services/search/images?q=$query&v=1.0&start=$i&rsz=8" | 
				sed 's/,/\n/g' | grep unescapedUrl | awk -F'"' '{print $4}')
		urls+=" "
	done
	echo "$urls" | sed 's/ /\n/g' > queries/query-"$query".txt
done

