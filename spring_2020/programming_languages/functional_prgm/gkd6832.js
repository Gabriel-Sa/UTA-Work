/*
Gabriel de Sa
1001676832
Feb 20, 2020
*/

function createTables(input, i, table, num, max)
//max should be the size of the array input
{
  if(i == max)
    return 1;
  createTables(input, i+1, table, num, max); // calls createTables again to iterate
  table[i] = input[i] * num; //creates the set of multiples
  return table; //returns the table
}

function square(input, table, i, max)
{
  if(i == max)
    return 0;
  square(input, table, i+1, max); // calls to iterate
  table[i] = input[i] * input[i]; // set the table to have the correct square.
  return table;
}

function oddMultiples(i, max, array, index, target, parity)
// max is the limit, index is for the array, target is the number, and parity is 0 for even and 1 for odd.
{
  if(i*target >= max) // base case
    return 0;

  if((target*i)%2 == parity) // check to see if the current number is even.
  {
    array[index] = target*i; //add this number to the array
    oddMultiples(i+1, max, array, index+1, target, parity); //recursion
  }
  else
  {
    oddMultiples(i+1, max, array, index, target, parity); //recursion with no index up
  }
  return table;
}

function sumMultiples(i, max, sum, target, parity)
// This function simply finds the multiples of 7 and then adds them together.
{
  if(i*target >= max)
    return 0;

  if(((i*target) % 2) == parity)
  {
    return sumMultiples(i+1, max, sum, target, parity) + (i*target); // in the ned will return the sum.
  }
  else
  {
    return sumMultiples(i+1, max, sum, target, parity); // basically adding 0 in this case.
  }
}

// making question 5 into a curried function
function cylinder_volume(r)
{ // this is a curried function that allows us to get one variable and return a function
  // that gets the next variable. Then calls a function that returns the volume.
  return (h) => {
    return 3.14 * r * r * h;
  }
}

var inputtable = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]; // initalize imputtable, and other arrays
var fiveTable = [];
var thirteenTable = [];
var squaresTable = [];
var oddTable = [];
console.log(inputtable); // Question 1 in generic mode. (generic mode = extra credit)

createTables(inputtable, 0, fiveTable, 5, 10); //Questino 2.a in generic mode
console.log(fiveTable);

createTables(inputtable, 0, thirteenTable, 13, 10); //Question 2.b in generic mode
console.log(thirteenTable);

square(inputtable, squaresTable, 0, 10); //Question 2.c in generic mode
console.log(squaresTable);

oddMultiples(1, 100, oddTable, 0, 5, 1); //Question 3 in generic mode
console.log(oddTable);

var total = sumMultiples(1, 100, 0, 7, 0); //Question 4 in generic mode
console.log(total);

console.log(cylinder_volume(5)(10)); //Question 5 answered here

/* Answering Question 6 here: Allowing us to use clourses to wrap the html without having to remmeber
 to close out the tag in html. It also allows us to print this text into the console. */

makeTag = function(beginTag, endTag)
{
  return function(textcontent)
  {
    return beginTag +textcontent +endTag;
  }
}

console.log(
  makeTag('<table>\n','</table>\n')(makeTag('\t<tr>\n','\t</tr>\n')(makeTag('\t<th>\n','\t</th>\n')('\t\tClass\n')))
  );
