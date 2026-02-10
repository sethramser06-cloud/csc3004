/* Bible class function definitions
 * Computer Science, MVNU
 * CSC-3004 Introduction to Software Development
 *
 * NOTE: You may add code to this file, but do not
 * delete any code or delete any comments.
 *
 * STUDENT NAME: Seth Ramser
 */

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// Default constructor
Bible::Bible()
{
   infile = "/home/class/csc3004/Bibles/web-complete";
}

// Constructor – pass bible filename
Bible::Bible(const string s) { infile = s; }

// REQUIRED: lookup finds a given verse in this Bible
Verse Bible::lookup(Ref ref, LookupResult &status)
{
   // TODO: scan the file to retrieve the line that holds ref ...
   // open the file
   instream.open(infile.c_str());
   // check for open error
   if (!instream)
   {
      cerr << "Error - can't open input file: " << infile << endl;
      exit(2);
   }

   string line;
   bool foundBook, foundChapter, foundVerse;
   foundBook = foundChapter = foundVerse = false;
   status = OTHER; // placeholder until retrieval is attempted

   // scan the file line by line
   while (getline(instream, line))
   {

      Ref currentRef = Ref(line);

      if (currentRef.getBook() == ref.getBook())
      {
         foundBook = true;
         if (currentRef.getChapter() == ref.getChapter())
         {
            foundChapter = true;
            if (currentRef.getVerse() == ref.getVerse())
            {
               foundVerse = true;
            }
         }
      }
      int pos = line.find(' ');           // find the first space after the reference
      string text = line.substr(pos + 1); // get the substring starting after the space

      // check if this is the requested reference
      if (currentRef == ref)
      {
         status = SUCCESS;
         return Verse(currentRef, text);
      }
      // else keep scanning til reference is found or file ends
   }
   if (!foundBook)
   {
      status = NO_BOOK;
   }
   else if (!foundChapter)
   {
      status = NO_CHAPTER;
   }
   else if (!foundVerse)
   {
      status = NO_VERSE;
   }
   instream.close();
   // create and return the verse object
   // default verse, to be replaced by a Verse object
   // that is constructed from a line in the file.
   return Verse();
}

// REQUIRED: Return the next verse from the Bible file stream if the file is open.
// If the file is not open, open the file and return the first verse.
Verse Bible::nextVerse(LookupResult &status)
{
   // open the file if not already open
   if (!instream.is_open())
   {
      instream.open(infile.c_str());
      if (!instream)
      {
         status = OTHER;
         return Verse();
      }
   }

   // read the next line from the file
   string line;
   if (!getline(instream, line))
   {
      status = OTHER;
      return Verse();
   }

   Ref currentRef = Ref(line);

   int pos = line.find(' ');           // find the first space after the reference
   string text = line.substr(pos + 1); // get the substring starting after the space

   // create and return the verse object
   status = SUCCESS;
   return Verse(currentRef, text);
}

// REQUIRED: Return an error message string to describe status
string Bible::error(LookupResult status)
{
   // return error message based on status
   switch (status)
   {
   case NO_BOOK:
      return "Error: No such book exists!";
      break;
   case NO_CHAPTER:
      return "Error: No such chapter exists!";
      break;
   case NO_VERSE:
      return "Error: No such verse exists!";
      break;
   default:
      return "Error: Unknown error!";
      break;
   }
}

void Bible::display()
{
   cout << "Bible file: " << infile << endl;
}

// OPTIONAL access functions
// OPTIONAL: Return the reference after the given ref
Ref Bible::next(const Ref ref, LookupResult &status)
{
   return ref;
}

// OPTIONAL: Return the reference before the given ref
Ref Bible::prev(const Ref ref, LookupResult &status)
{
   return ref;
}
