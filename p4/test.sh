#!/bin/bash
FAIL=0

# Function to run the program against a test case and check
# its output and exit status for correct behavior
testShopping() {
  TESTNO=$1

  rm -f output.txt outlist.txt

  echo "Test $TESTNO: ./shopping < input-$TESTNO.txt > output.txt 2> stderr.txt"
  ./shopping < input-$TESTNO.txt > output.txt
  STATUS=$?

  # Make sure the program exited successfully
  if [ $STATUS -ne 0 ]
  then
      echo "**** Test $TESTNO FAILED - incorrect exit status"
      FAIL=1
      return 1
  fi
  
  # Make sure any output to standard out looks right.
  if ! diff -q expected-$TESTNO.txt output.txt >/dev/null 2>&1
  then
      echo "**** Test $TESTNO FAILED - output didn't match the expected output"
      FAIL=1
      return 1
  fi

  if [ -f expected-list-$TESTNO.txt ] ; then
      if ! diff -q expected-list-$TESTNO.txt outlist.txt >/dev/null 2>&1
      then
	  echo "**** Test $TESTNO FAILED - saved shopping list in outlist.txt isn't right"
	  FAIL=1
	  return 1
      fi
  fi

  echo "Test $TESTNO PASS"
  return 0
}

# make a fresh copy of the target programs
make clean
make

if [ -x shopping ] ; then
    testShopping 01
    testShopping 02
    testShopping 03
    testShopping 04
    testShopping 05
    testShopping 06
    testShopping 07
    testShopping 08
    testShopping 09
    testShopping 10
    testShopping 11
    testShopping 12
    testShopping 13
    testShopping 14
    testShopping 15
    testShopping 16
    testShopping 17
    testShopping 18
else
    echo "**** Your program didn't compile successfully, so we couldn't test it."
    FAIL=1
fi

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  echo "Tests successful"
  exit 0
fi
