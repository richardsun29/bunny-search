#!/bin/bash

# Array of queries to search
#declare -a queries=('bunny' 'rabbit' 'cute bunny' 'bunny gif')

# Set up working directory
cd "$(dirname "${BASH_SOURCE[0]}")" && cd "$(git rev-parse --show-toplevel)" # pwd -> git repo's root
! [ -d "queries/" ] && mkdir queries/

HELP() {
echo "Usage: geturls.sh [OPTIONS] [QUERY] ... 
Retrieve a lists of URLs for each QUERY.

Mandatory arguments to long options are mandatory for short options too.
  -c, --count=NUM           Number of results for each query (max: 64)
  -f, --force-refresh       Overwrite cached results
  -h, --help                Show this help text
"
}

# Command line arguments
USE_CACHE=true
nitems=64
if ! options=$(getopt -o c:fh -l count:,force-refresh,help -- "$@"); then
	HELP
	exit 1;
fi
eval set -- $options

while [ $# -gt 0 ]; do
	case "$1" in
		-c | --count) 
			nitems="$2"
			shift
			if [[ ! $nitems =~ ^[0-9]+$ ]]; then
				echo "geturls: --count: invalid results count: $nitems" >&2
				exit 1
			fi
			if [[ $nitems -gt 64 ]]; then
				echo "geturls: --count: Maximum number of results is 64." >&2
				exit 1
			fi
			;;
		-f | --force-refresh) USE_CACHE=false;;
		-h | --help) HELP; exit 0;;
		(--) ;;
		(-*) echo "$0: unrecognized option $1" >&2; exit 1;;
		(*) queries[${#queries[@]}]="$1";;
	esac
	shift
done


if [ ${#queries[@]} -eq 0 ]; then
	echo "geturls: no queries. Exiting." >&2
	exit 1
fi
# Replace spaces with URL escape characters
for ((i = 0; i < ${#queries[@]}; i++)); do
	queries["$i"]=$(echo "${queries[$i]}" | sed -e 's/ /%20/g')
done


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
	printf "Retrieving results for \'%s\'..." $query

	# Cache results
	if [[ "$USE_CACHE" = "true" && -f queries/"$query".txt ]]; then
		i=$(wc -l queries/"$query".txt 2>/dev/null | awk '{print $1}')
		cached_results="($i cached)"
	else
		rm -f queries/"$query".txt 2>/dev/null
		i=0
		cached_results=
	fi


	urls=""
	for ((; i < nitems-8; i+=8)); do
		# Loading percent
		percent=$(bc <<< "scale = 3; $i/$nitems*100" | awk -F. '{print $1}')
		printf "%.0f%%" $percent

		# Extract URLs
		urls+="$(extract $query $i 8) "

		# Erase current percentage
		[ "$percent" -lt 10 ] && printf "\b\b" || printf "\b\b\b"
	done

	# Cleanup iteration
	if [ "$i" -ne "$nitems" ]; then
		urls+=$(extract "$query" "$i" "$(($nitems-$i))" )
	fi

	if [ -n "$urls" ]; then
		echo "$urls" | sed -e 's/\s*$//' -e 's/ /\n/g' >> queries/"$query".txt
	fi
	printf "100%% %s\n" "$cached_results"
done

