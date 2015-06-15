#!/bin/bash

# Array of queries to search
declare -a queries=('bunny' 'rabbit' 'cute bunny' 'bunny gif')
#declare -a queries=('bunny')

# Number of items to search for
# Rounds to a multiple of 8
# Maximum: 64
nitems=100
[ "$nitems" -gt 64 ] && nitems=64

# Command line arguments
CACHE=true
if ! options=$(getopt -o f -l force-refresh -- "$@"); then
	# TODO: add help text
	exit 1;
fi
eval set -- $options
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


# Returns Google Search API's response
# Arguments:
# $1: query
# $2: starting index (max 64 - $3)
# $3: number of results (max 8)
extract() {
		echo $(curl -s "http://ajax.googleapis.com/ajax/services/search/images?q=$1&v=1.0&start=$2&rsz=$3" | 
				sed 's/,/\n/g' | grep unescapedUrl | awk -F'"' '{print $4}')
}

# Get image URLs
for query in ${queries[@]}; do
	printf "Retrieving search results for %s..." $query

	# Cache results unless --force-refresh
	# If no cached results, i == "" which counts as 0
	if [ "$CACHE" = "true" ]; then
		i=$(wc -l queries/"$query".txt 2>/dev/null | awk '{print $1}')
	else
		i=0
		rm -f queries/"$query".txt 2>/dev/null
	fi


	urls=""
	for ((; i < (nitems-8) && i <= 56; i+=8)); do
		# Loading percent
		percent=$(bc <<< "scale = 3; $i/$nitems*100" | awk -F. '{print $1}')
		printf "%.0f%%" $percent

		# Extract URLs
		urls+="$(extract $query $i 8) "

		# Erase current percentage
		[ $percent -lt 10 ] && printf "\b\b" || printf "\b\b\b"
	done

	# Cleanup iteration
	[ "$i" = "$nitems" ] || urls+=$(extract "$query" "$i" $(($nitems-$i)) )

	echo "$urls" | sed -e 's/\s*$//' -e 's/ /\n/g' >> queries/"$query".txt
	printf "100%%\n"
done


