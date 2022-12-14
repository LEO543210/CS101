/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include <iostream>
#include <cmath>
#include "testing/SimpleTest.h"
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* This function take a better method to get divisor sum of n.
 * We take advantage of the fact that every divisor less than the
 * square root is paired with a divisor greater than the square
 * root.
 */
long smarterSum(long n) {
    /* TODO: Fill in this function. */
    long total = 0;
    for(long divisor = 1; divisor <= sqrt(n); divisor++) {
        if(n == 1){
            total = 0;
            break;
        }
        if(n % divisor == 0){
            if(n / divisor == n){
                total += 1;
            }
            else{
               total = total + (divisor + n / divisor);
            }
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * The different point is this function using smarterSum() to
 * replace divisorSum().
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 *
 */
bool isPerfectSmarter(long n) {
    /* TODO: Fill in this function. */
    return (n != 0) && (n == smarterSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfectsSmarter(long stop) {
     /* TODO: Fill in this function. */
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* This funtion get 'n' and return the nTH perfect number.
 *
 */
long findNthPerfectEuclid(long n) {
    /* TODO: Fill in this function. */
    int count = 0;
    int k = 1;
    long perfectNumber = 0;
    while(count < n){
        long m = pow(2, k) - 1;
        if(smarterSum(m) == 1){
            perfectNumber = pow(2, k-1) * m;
            count++;
        }
        k++;
    }
    return perfectNumber;
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfects(10000));
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
}

// TODO: add your student test cases here

STUDENT_TEST("Time trials of findPerfects on doubling input sizes"){
    TIME_OPERATION(80000, findPerfects(80000));
    TIME_OPERATION(160000, findPerfects(160000));
}

STUDENT_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(smarterSum(25),11);
    EXPECT_EQUAL(smarterSum(49),15);
    EXPECT_EQUAL(smarterSum(101),1);
    EXPECT_EQUAL(smarterSum(1),0);
    EXPECT_EQUAL(smarterSum(6),6);
    EXPECT_EQUAL(smarterSum(12),16);
}

STUDENT_TEST("Test -6 and -1000(negative number) are perfect."){
    EXPECT(isPerfect(-6));
    EXPECT(isPerfect(-1000));
}

STUDENT_TEST("Time trials of findPerfectsSmarter on doubling input sizes") {
    TIME_OPERATION(10000, findPerfectsSmarter(10000));
    TIME_OPERATION(20000, findPerfectsSmarter(20000));
    TIME_OPERATION(40000, findPerfectsSmarter(40000));
    TIME_OPERATION(80000, findPerfectsSmarter(80000));
    TIME_OPERATION(160000, findPerfectsSmarter(160000));
}

STUDENT_TEST("Confirm perfect number.") {
    EXPECT_EQUAL(findNthPerfectEuclid(1), 6);
    EXPECT_EQUAL(findNthPerfectEuclid(2), 28);
    EXPECT_EQUAL(findNthPerfectEuclid(3), 496);
    EXPECT_EQUAL(findNthPerfectEuclid(4), 8128);
    EXPECT_EQUAL(findNthPerfectEuclid(5), 33550336);
    EXPECT(isPerfect(findNthPerfectEuclid(5)));
}
