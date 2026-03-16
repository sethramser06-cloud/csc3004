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
#include <algorithm>

using namespace std;

// Default constructor
Bible::Bible()
{
   // Constructor builds the reference index when the Bible object is created
   infile = "/home/class/csc3004/Bibles/web-complete";
   buildTextIndex(infile);
}

// Constructor – pass bible filename
Bible::Bible(const string s)
{
   infile = s;
   buildTextIndex(infile);
}

/*
   Builds an index that maps each Bible reference
   to its byte offset in the text file.
   This allows fast lookup using seekg()
*/
int Bible::buildTextIndex(string filename)
{
   // Open the file
   ifstream infile(filename.c_str());
   // Check to make sure file is open
   if (!infile)
   {
      cerr << "Error - can't open input file: " << filename << endl;
      return 0;
   }

   string line;

   // Capture starting position in file
   streampos pos = infile.tellg();

   // Used to store data about the number of verses retrieved and
   // the last offset gathered at the end of the file
   int count = 0;
   int lastOffset = 0;

   while (getline(infile, line))
   {
      // Convert streampos into int explicitly
      int position = static_cast<int>(pos);
      // Create a Ref from the current line, extracting book, chapter, and verse
      Ref currentRef(line);
      // Inserts the ref into the map
      refs.insert({currentRef, position});

      // Stores the last byte offset read, then updates the amount of
      // verses so far
      lastOffset = position;
      count++;

      // Update position for next line
      pos = infile.tellg();
   }

   cout << "Index built successfully." << endl;
   cout << "Total references indexed: " << count << endl;
   cout << "Last byte offset added: " << lastOffset << endl;
   return 1;
}

// Use the index to find the byte offset of the requested reference.
// Then use seekg() to jump directly to that location in the file.
Verse Bible::lookup(Ref ref, LookupResult &status)
{
   // Initialize status to OTHER to tell us if something goes wrong.
   // Then, search the map for the key ref
   status = OTHER;
   auto it = refs.find(ref);

   // If the ref isn't found, then the exact book/chapter/index
   // doesn't exist in the index.
   if (it == refs.end())
   {
      bool foundBook = false;
      bool foundChapter = false;

      // Loops through the entire map to see if the book or chapter
      // is correct to find the correct error to display
      for (auto mapIt = refs.begin(); mapIt != refs.end(); ++mapIt)
      {
         Ref currentRef = mapIt->first;

         if (currentRef.getBook() == ref.getBook())
         {
            foundBook = true;

            if (currentRef.getChapter() == ref.getChapter())
            {
               foundChapter = true;
               break;
            }
         }
      }

      // Assign correct error code
      if (!foundBook)
      {
         status = NO_BOOK;
      }
      else if (!foundChapter)
      {
         status = NO_CHAPTER;
      }
      else
      {
         status = NO_VERSE;
      }

      // return empty verse to prevent crashes
      return Verse();
   }

   // Now that the iterator was found, we get the byte offset
   int position = it->second;

   // Opens the file if needed, using instream instead of reopening the
   // file every time
   if (!instream.is_open())
   {
      instream.open(infile.c_str());
      if (!instream)
      {
         status = OTHER;
         return Verse();
      }
   }

   // Clears the stream state in case previous read hit EOF
   instream.clear();
   // Jump directly to verse
   instream.seekg(position);

   string line;
   // Read in exactly one line (may need to change for multiple verse retrieval)
   if (!getline(instream, line))
   {
      status = OTHER;
      return Verse();
   }

   // Separates the text from the reference to skip numbers
   int pos = line.find(' ');
   string text = line.substr(pos + 1);

   // Set status to success and return the Verse object
   status = SUCCESS;
   return Verse(ref, text);
}

// Gets the next verse if requested (functional but isn't called yet in
// testreader.cpp)
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
