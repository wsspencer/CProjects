#!/bin/bash
FAIL=0

# Function to run the magic program against a test case and check
# its output and exit status for correct behavior
testMagic() {
  TESTNO=$1
  ESTATUS=$2

  rm -f output.txt

  echo "Magic test $TESTNO: ./magic < input-m$TESTNO.txt > output.txt"
  ./magic < input-m$TESTNO.txt > output.txt
  STATUS=$?

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ] ; then
      echo "**** Magic test $TESTNO FAILED - incorrect exit status. Expected: $ESTATUS Got: $STATUS"
      FAIL=1
      return 1
  fi

  # Make sure the output matches the expected output.
  
  if ! diff -q expected-m$TESTNO.txt output.txt >/dev/null 2>&1 ; then
      echo "**** Magic test $TESTNO FAILED - output didn't match the expected output"
      FAIL=1
      return 1
  fi

  echo "Magic test $TESTNO PASS"
  return 0
}

# Function to run the frame program against a test case and check its
# output and exit status for correct behavior
testFrame() {
  TESTNO=$1
  ESTATUS=$2

  rm -f output.ppm

  echo "Frame test $TESTNO: ./frame < input-f$TESTNO.ppm > output.ppm"
  ./frame < input-f$TESTNO.ppm > output.ppm
  STATUS=$?

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** Frame test $TESTNO FAILED - incorrect exit status. Expected: $ESTATUS Got: $STATUS"
      FAIL=1
      return 1
  fi

  # For this program, we don't care about the output if it has to exit
  # unsuccessfully
  if [ $ESTATUS -eq 0 ] ; then
      # Make sure the output matches the expected output.
      if ! diff -q expected-f$TESTNO.ppm output.ppm >/dev/null 2>&1 ; then
	  echo "**** Frame test $TESTNO FAILED - output didn't match the expected output"
	  FAIL=1
	  return 1
      fi
  fi

  echo "Frame test $TESTNO PASS"
  return 0
}

# make a fresh copy of the target programs
make clean
make

# Run individual tests.

if [ -x magic ] ; then
    testMagic 1 0
    testMagic 2 0
    testMagic 3 0
    testMagic 4 0
    testMagic 5 0
else
    echo "**** Magic program didn't compile successfully"
    FAIL=1
fi

if [ -x frame ] ; then
    testFrame 1 0
    testFrame 2 0
    testFrame 3 0
    testFrame 4 0
    testFrame 5 0
    testFrame 6 100
    testFrame 7 101
    testFrame 8 102
else
    echo "**** Magic program didn't compile successfully"
    FAIL=1
fi

if [ $FAIL -ne 0 ]; then
  echo "**** There were failing tests"
  exit 1
else 
  echo "Tests successful"
  exit 0
fi
