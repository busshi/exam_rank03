#!/bin/bash

exit_status=0

cd micro_paint && /bin/bash checker.sh
[[ $? -ne 0 ]] && exit_status=$(( $exit_status + 1 ))

echo -e "\n\n"

cd ../mini_paint && /bin/bash checker.sh
[[ $? -ne 0 ]] && exit_status=$(( $exit_status + 1 ))

exit $exit_status
