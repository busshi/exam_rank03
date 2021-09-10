#!/bin/bash

green="\033[32;1m"
red="\033[31;1m"
blue="\033[36m"
orange="\033[33m"
clear="\033[0m"

OK="[ ${green}OK${clear} ]"
KO="[ ${red}KO${clear} ]"

ko=0
ok=0


### HEADER
echo -e "${orange}____________________________________________________________________________________________________________\n"
echo -e "_____________________________________________ MICRO_PAINT CHECKER __________________________________________\n"
echo -e "____________________________________________________________________________________________________________\n\n"


### COMPILATION
echo -e "${orange}Compilation...${clear}"
clang -Wall -Wextra -Werror micro_paint.c
[[ $? -ne 0 ]] && { echo -e ${KO}; exit 1; } || { ok=$(( $ok + 1 )); echo -e ${OK}; }


### TESTS ARGUMENTS
echo -e "\n${orange}Testing no argument...${clear}"
./a.out > out
[[ $? -ne 1 ]] && { echo -e "Return value\t${KO}"; ko=$(( $ko + 1 )); } || { echo -e "Return value\t${OK}"; ok=$(( $ok + 1 )); }
[ "$(cat -e out)" != 'Error: argument$' ] && { echo -e "Output\t\t${KO}"; ko=$(( $ko + 1 )); } || { echo -e "Output\t\t${OK}"; ok=$(( $ok + 1 )); }


echo -e "\n${orange}Testing too many arguments...${clear}"
./a.out examples/0 examples/1 > out
[[ $? -ne 1 ]] && { echo -e "Return value\t${KO}"; ko=$(( $ko + 1 )); } || { echo -e "Return value\t${OK}"; ok=$(( $ok + 1 )); }
[ "$(cat -e out)" != 'Error: argument$' ] && { echo -e "Output\t\t${KO}"; ko=$(( $ko + 1 )); } || { echo -e "Output\t\t${OK}"; ok=$(( $ok + 1 )); }


echo -e "\n${orange}Testing wrong file...${clear}"
./a.out wrong_file > out
[[ $? -ne 1 ]] && { echo -e "Return value\t${KO}"; ko=$(( $ko + 1 )); } || { echo -e "Return value\t${OK}"; ok=$(( $ok + 1 )); }
[ "$(cat -e out)" != 'Error: Operation file corrupted$' ] && { echo -e "Output\t\t${KO}"; ko=$(( $ko + 1 )); } || { echo -e "Output\t\t${OK}"; ok=$(( $ok + 1 )); }



### WRONG VALUES
wrong_test()
{
echo -e "$2" > test
./a.out test > out
ret=$?

if [ "$1" = "output" ] ; then
	[ "$(cat -e out)" != 'Error: Operation file corrupted$' ] && { echo -e "${KO}\c"; ko=$(( $ko + 1 )); } || { echo -e "${OK}\c"; ok=$(( $ok + 1 )); }
elif [ "$1" = "return_value" ]; then
	[[ $ret -ne 1 ]] && { echo -e "${KO}\c"; ko=$(( $ko + 1 )); } || { echo -e "${OK}\c"; ok=$(( $ok + 1 )); }
fi

rm -f out test
}


### WRONG VALUE 1st LINE
echo -e "\n${orange}Testing output and return value with wrong values: 1st line (WIDTH HEIGHT BACKGROUND_CHAR)...${clear}"

echo -e "Return value\t\c"
for line in "0 100 c" "301 100 c" "1 0 c" "-301 100 c" "1 -1 c" "100 301 c" "100 100" "100" ""; do
	wrong_test return_value ${line}
done

echo -e "\nOutput\t\t\c"
for line in "0 100 c" "301 100 c" "1 0 c" "-301 100 c" "1 -1 c" "100 301 c" "100 100" "100" ""; do
        wrong_test output ${line}
done


### WRONG VALUE OTHER LINES
echo -e "\n\n${orange}Testing output and return value with wrong values: other lines (r/R X Y WIDTH HEIGHT CHAR)...${clear}"

echo -e "Return value\t\c"
for line in "110 100 c\na" "100 100 c\nr -2" "100 100 c\nR 12.1 -2" "100 100 c\nr 11.1 22.2 33.3 44.4 @" "100 100 c\nr 1.1 2.2 3.3 4.4 -2 1 c" "100 100 c\nb 1.1 2.2 3.3 4.4 c" "100 100 c\nR 2.0 1.0 3.0 -4.0 c" "100 100 c\nR 0 1 2 3 d" "100 100 c\nr 2 3 4 0 e"; do
        wrong_test return_value ${line}
done

echo -e "\nOutput\t\t\c"
for line in "110 100 c\na" "100 100 c\nr -2" "100 100 c\nR 12.1 -2" "100 100 c\nr 11.1 22.2 33.3 44.4 @" "100 100 c\nr 1.1 2.2 3.3 4.4 -2 1 c" "100 100 c\nb 1.1 2.2 3.3 4.4 c" "100 100 c\nR 2.0 1.0 3.0 -4.0 c" "100 100 c\nR 0 1 2 3 d" "100 100 c\nr 2 3 4 0 e"; do
        wrong_test output ${line}
done

### TESTING EXAMPLES
echo -e "\n\n${orange}Testing examples...${clear}"
test()
{
./a.out "examples/$1" > "mine$1"
ret=$?
if [ "$2" = "output" ] ; then
	diff "outputs/test$1" "mine$1" &> /dev/null
	[[ $? -eq 0 ]] && { echo -e "${OK}\c"; ok=$(( $ok + 1 )); } || { echo -e "${KO}\c"; ko=$(( $ko + 1 )); }
elif [ "$2" = "return_value" ] ; then
	[[ $ret -eq 0 ]] && { echo -e "${OK}\c"; ok=$(( $ok + 1 )); } || { echo -e "${KO}\c"; ko=$(( $ko + 1 )); }
fi
rm -f "mine$1"
}

echo -e "Return value\t\c"
for nb in 0 1 2 3 4 5 6 7 8; do
	test ${nb} return_value
done

echo -e "\nOutput\t\t\c"
for nb in 0 1 2 3 4 5 6 7 8; do
        test ${nb} output
done



### CLEANING
rm -f a.out


### SUMMARY
total=$(( $ok + $ko ))
echo -e "\n\n\n${green}Passed: ${ok} / ${total}"
echo -e "${red}Failed: ${ko} / ${total}"

exit $ko
