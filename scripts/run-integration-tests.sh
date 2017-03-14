#!/bin/bash

PROJECT_DIR=$(cd `dirname $0`/..; pwd)
TESTS_DIR=$PROJECT_DIR/integration-tests
BUILD_DIR=$PROJECT_DIR/target

function runTest() {
  TEST_DIR=$1
  TEST_NAME=$(basename $TEST_DIR)
  TEST_TMP_DIR=$BUILD_DIR/$(mktemp -u -d --suffix=-mechanic-it)
  mkdir -p $TEST_TMP_DIR
  echo "Running test $TEST_NAME... (TEST_TMP_DIR: $TEST_TMP_DIR)"
  TEST_TMP_RESULT=$BUILD_DIR/$(mktemp -u --suffix=-mechanic-it)
  mkdir -p $(dirname $TEST_TMP_DIR)
  tar xfz $BUILD_DIR/bundle.tgz -C $TEST_TMP_DIR
  if [ -d $TEST_DIR/input/ ]; then
    cp -R $TEST_DIR/input/* $TEST_TMP_DIR
  fi
  #find $TEST_TMP_DIR
  for i in 1 2; do
    echo "Run #$i"
    TEST_ROOT=$TEST_TMP_DIR \
	MECHANIC_ROOT_DIR=$TEST_TMP_DIR \
  	$TEST_TMP_DIR/usr/bin/mechanic list-migrations --order-by=id | cut --delimiter='	' -f 1,2,5 >> $TEST_TMP_RESULT
    if [[ ! -f "$TEST_DIR/run-test.sh" ]]; then
      TEST_ROOT=$TEST_TMP_DIR \
	MECHANIC_ROOT_DIR=$TEST_TMP_DIR \
  	$TEST_TMP_DIR/usr/bin/mechanic -v migrate -- /bin/true
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
  	$TEST_TMP_DIR/usr/bin/mechanic list-migrations --order-by=id | cut --delimiter='	' -f 1,2,5 >> $TEST_TMP_RESULT
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
}

if [ $# = 0 ]; then
  for TEST_DIR in $(find $TESTS_DIR -mindepth 1 -maxdepth 1 -type d); do
    runTest $TEST_DIR
  done
else
  for TEST_DIR in $*; do
    runTest $TESTS_DIR/$TEST_DIR
  done
fi

exit 0
