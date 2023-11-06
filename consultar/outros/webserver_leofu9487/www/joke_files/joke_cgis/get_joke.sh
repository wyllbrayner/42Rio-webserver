DIR_NAME="www/joke_files/jokes"
STR="$QUERY_STRING"
printf "Content-Type: text/plain\r\n\n"

IFS="="
set $STR
JOKE="joke_$2"
FILE="$DIR_NAME/$JOKE"

echo "$JOKE :\n\n"

if [ -f "$FILE" ]
then
	cat $FILE
else
	echo "This joke doesn't exist"
fi