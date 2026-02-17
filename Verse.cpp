/* Verse class function definitions
 * Computer Science, MVNU
 * CSC-3004 Introduction to Software Development
 *
 * NOTE: You may add code to this file, but do not
 * delete any code or delete any comments.
 *
 * STUDENT NAME: Seth Ramser
 */

#include "Verse.h"
#include <iostream>
#include <sstream>

using namespace std;

// Default constructor
Verse::Verse()
{
   verseText = "Uninitialized Verse!";
   verseRef = Ref();
}

// REQUIRED: Parse constructor - pass verse string from file
Verse::Verse(const string s)
{
   // TODO: use Ref constructor to create verseRef
   verseRef = Ref(s);

   // Find where the reference ends and the text begins
   int pos = s.find(' ');         // find the first space after the reference
   verseText = s.substr(pos + 1); // get the substring starting after the space

   // remove leading space if it exists
   if (!verseText.empty() && verseText[0] == ' ')
   {
      verseText.erase(0, 1);
   }
}

// Constructor for Verse given Ref and text
Verse::Verse(Ref r, string text)
{
   verseRef = r;
   verseText = text;

   // remove leading space if it exists
   if (!verseText.empty() && verseText[0] == ' ')
   {
      verseText.erase(0, 1);
   }
}

// REQUIRED: Accessors
string Verse::getVerse()
{
   return verseText;
}

Ref Verse::getRef()
{
   return verseRef;
}

// display reference and verse
void Verse::display()
{
   verseRef.display();
   cout << verseText << endl;
}

void Verse::displayNext()
{
   // displays only the verse number and the text on the same line
   verseRef.displayNext();
   cout << verseText << endl;
}
