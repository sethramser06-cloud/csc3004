/* testreader.cpp
 * Computer Science, MVNU
 * CSC-3004 Introduction to Software Development
 *
 * main function for Project 3 Part 1
 *
 * STUDENT NAME: Seth Ramser
 */

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv)
{
   // Right now, multiple verse retrieval is not possible, so there should only
   // ever be exactly 4 parameters.
   if (argc != 4 && argc != 5)
   {
      cout << "Usage: " << argv[0] << " <book#> <chapter#> <verse#> <OPTIONAL: number of verses>" << endl;
      return 1;
   }

   // Initialize book, chapter, and verseNum ints
   int book = atoi(argv[1]);
   int chapter = atoi(argv[2]);
   int verseNum = atoi(argv[3]);
   int numVerses = 1;
   if (argc == 5)
   {
      numVerses = atoi(argv[4]);
   }

   cout << "Creating Bible and building index..." << endl;

   // Create Bible object to process the raw text file
   Bible webBible("/home/class/csc3004/Bibles/web-complete");

   cout << endl;

   // Create Ref object and result object
   Ref ref(book, chapter, verseNum);
   LookupResult result;

   cout << "Looking up reference(s) using index..." << endl;

   // Uses the new lookup function to get the requested verse
   Verse verse = webBible.lookup(ref, result);

   // Error handling for if the requested verse does not exist
   if (result != SUCCESS)
   {
      cout << webBible.error(result) << endl;
      return 1;
   }

   // Display the verse
   verse.display();

   Verse prevVerse;
   for (int i = 1; i < numVerses; i++)
   {
      prevVerse = verse;
      verse = webBible.nextVerse(result);
      if (result != SUCCESS)
      {
         cout << webBible.error(result) << endl;
         return 1;
      }
      if ((verse.getRef().getBook() > prevVerse.getRef().getBook()) ||
          verse.getRef().getChapter() > prevVerse.getRef().getChapter())
      {
         verse.display();
      }
      else
      {
         verse.displayNext();
      }
   }
   return 0;
}
