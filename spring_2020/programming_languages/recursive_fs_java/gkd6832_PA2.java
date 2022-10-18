/*
Gabriel de Sa
1001676832
Lab 2 March 31, 2020
Mac OS Catalina Java Version "14" 2020-03-17
*/
import java.io.File; //Calling the file library.

public class gkd6832_PA2 {

  public static void main(String[] args)
  {
    String currentDir = System.getProperty("user.dir"); //Using System.getProperty we can get the current directory.
    File f = new File(currentDir); //Make a base file with the current directory
    File[] files = f.listFiles(); //Make a list of Files
    long totalSize = 0;
    gkd6832_PA2 object = new gkd6832_PA2(); //New object so we can do recursion.
    totalSize = object.getTotalSize(files); //start the recursion process
    System.out.println("The total size of this directory and all files and subdirectories is: " + totalSize + " Bytes");
  }

  long getTotalSize(File[] files)
  {
    long size = 0;
    for(File x : files) //range based for loop
    {
      if(x.isDirectory())  //Check if file is a directory
      {
        size += getTotalSize(x.listFiles()); //if is branch to another iteration of getTotalSize.
      }
      else if(x.isFile()) //if its a file simply add the size to the total.
      {
        size += x.length();
      }
    }
    return size; //return to caller.
  }

}
