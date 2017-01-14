#!/bin/bash
#
# Copyright (c) 2016 the Server Mechanic Team.
# Server Mechanic (http://server-mechanic.org)
#
# This file is part of Server Mechanic.
#
# Server Mechanic is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
#

PROJECT_DIR=$(cd `dirname $0`/..; pwd)
TESTS_DIR=$PROJECT_DIR/integration-tests
BUILD_DIR=$PROJECT_DIR/target

for TEST_DIR in $(find $TESTS_DIR -mindepth 1 -maxdepth 1 -type d); do
  TEST_NAME=$(basename $TEST_DIR)
  TEST_TMP_DIR=$(mktemp -d --suffix=-mechanic-it)
  echo "Running test $TEST_NAME... (TEST_TMP_DIR: $TEST_TMP_DIR)"
  TEST_TMP_RESULT=$(mktemp --suffix=-mechanic-it)
  cp -R $TEST_DIR/input/* $TEST_TMP_DIR
  mkdir -p $TEST_TMP_DIR/usr/sbin/
  cp $BUILD_DIR/bin/mechanic $TEST_TMP_DIR/usr/sbin/
  for i in 1 2; do
    echo "Run #$i"
    if [[ ! -f "$TEST_DIR/run-test.sh" ]]; then
      TEST_ROOT=$TEST_TMP_DIR \
	MECHANIC_ROOT_DIR=$TEST_TMP_DIR \
  	$TEST_TMP_DIR/usr/sbin/mechanic -v migrate -- /bin/true
      TEST_EXIT_CODE=$?
    else
      TEST_ROOT=$TEST_TMP_DIR \
	MECHANIC_ROOT_DIR=$TEST_TMP_DIR \
  	$TEST_DIR/run-test.sh
      TEST_EXIT_CODE=$?
    fi
    echo $TEST_EXIT_CODE > $TEST_TMP_RESULT
    cd $TEST_TMP_DIR
    find . | grep -v /usr | sort -V >> $TEST_TMP_RESULT
    TEST_ROOT=$TEST_TMP_DIR \
	MECHANIC_ROOT_DIR=$TEST_TMP_DIR \
  	$TEST_TMP_DIR/usr/sbin/mechanic list-migrations | cut --delimiter='	' -f 1,2,5 >> $TEST_TMP_RESULT
    #cat $TEST_TMP_RESULT
    diff $TEST_DIR/output $TEST_TMP_RESULT
    DIFF_EXIT_CODE=$?
    echo -n "Test $TEST_NAME "
    if [ "0" = "$DIFF_EXIT_CODE" ]; then
      echo "OK."
    else
      echo "FAILED."
      exit 1
    fi
  done
done
exit 0
