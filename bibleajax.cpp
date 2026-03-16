/* Demo server program for Bible lookup using AJAX/CGI interface
 * By James Skon, Febrary 10, 2011
 * updated by Bob Kasper, January 2020
 * updated by Deborah Wilson, February 2026
 * Mount Vernon Nazarene University
 *
 * This sample program works using the cgicc AJAX library to
 * allow live communication between a web page and a program running on the
 * same server that hosts the web server.
 *
 * This program is run by a request from the associated html web document.
 * A Javascript client function invokes an AJAX request,
 * passing the input form data as the standard input string.
 *
 * The cgi.getElement function parses the input string, searching for the matching
 * field name, and returing a "form_iterator" oject, which contains the actual
 * string the user entered into the corresponding field. The actual values can be
 * accessed by dereferencing the form iterator twice, e.g. **verse
 * refers to the actual string entered in the form's "verse" field.
 *
 * STUDENT NAME: Seth Ramser
 */

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"

using namespace std;
using namespace cgicc;

int main()
{
   /* A CGI program must send a response header with content type
    * back to the web client before any other output.
    * For an AJAX request, our response is not a complete HTML document,
    * so the response type is just plain text to insert into the web page.
    */
   cout << "Content-Type: text/plain\n\n";

   Cgicc cgi; // create object used to access CGI request data

   // GET THE INPUT DATA
   // browser sends us a string of field name/value pairs from HTML form
   // retrieve the value for each appropriate field name
   form_iterator version = cgi.getElement("version");
   form_iterator bookIt = cgi.getElement("book");
   form_iterator chapterIt = cgi.getElement("chapter");
   form_iterator verseIt = cgi.getElement("verse");
   form_iterator nv = cgi.getElement("num_verse");

   // TODO: OTHER INPUT VALUE CHECKS ARE NEEDED ... but that's up to you!

   /* TODO: PUT CODE HERE TO CALL YOUR BIBLE CLASS FUNCTIONS
    *        TO LOOK UP THE REQUESTED VERSES
    */
   // Create Bible object to process the raw text file
   Bible curBible;
   switch (version->getIntegerValue())
   {
   case 1:
      curBible = Bible("/home/class/csc3004/Bibles/web-complete");
      break;
   case 2:
      curBible = Bible("/home/class/csc3004/Bibles/kjv-complete");
      break;
   case 3:
      curBible = Bible("/home/class/csc3004/Bibles/dby-complete");
      break;
   case 4:
      curBible = Bible("/home/class/csc3004/Bibles/ylt-complete");
      break;
   case 5:
      curBible = Bible("/home/class/csc3004/Bibles/webster-complete");
      break;
   default:
      cout << "Error! Version does not exist!";
      return 0;
   }

   Verse currentVerse;
   int bookNum, chapterNum, verseNum, numVerses;
   LookupResult result;

   bookNum = bookIt->getIntegerValue();
   chapterNum = chapterIt->getIntegerValue();
   verseNum = verseIt->getIntegerValue();

   if (nv != cgi.getElements().end())
   {
      numVerses = nv->getIntegerValue();
   }
   else
   {
      numVerses = 1;
   }

   Ref ref(bookNum, chapterNum, verseNum);
   currentVerse = curBible.lookup(ref, result);

   if (result != SUCCESS)
   {
      switch (result)
      {
      case NO_BOOK:
         cout << "<p>Book number " << ref.getBook() << " does not exist.</p>" << endl;
         return -1;
      case NO_CHAPTER:
         cout << "<p>Chapter number " << ref.getChapter() << " does not exist in "
              << ref.getBookName(ref.getBook()) << ".</p>" << endl;
         return -2;
      case NO_VERSE:
         cout << "<p>Verse number " << ref.getVerse() << " does not exist in "
              << ref.getBookName(ref.getBook()) << " chapter " << ref.getChapter() << ".</p>" << endl;
         return -3;
      default:
         cout << "<p>An unknown error occurred.</p>" << endl;
         return -4;
      }
   }

   currentVerse.display();

   for (int i = 1; i < numVerses; i++)
   {
      int prevChapter = currentVerse.getRef().getChapter();
      int prevBook = currentVerse.getRef().getBook();
      currentVerse = curBible.nextVerse(result);

      if (result != SUCCESS)
      {
         cout << curBible.error(result) << endl;
         break;
      }

      if (prevChapter < currentVerse.getRef().getChapter())
      {
         cout << "<br/>"; // add a line break between chapters
         currentVerse.display();
      }
      else if (prevBook < currentVerse.getRef().getBook())
      {
         cout << "<br/><br/>"; // add a line break between books
         currentVerse.display();
      }
      else
      {
         currentVerse.displayNext();
      }
   }
   // cout << "<p>Result status: " << result << "</p>" << endl;
   cout << endl;
   /* SEND BACK THE RESULTS
    * Finally we send the result back to the client on the standard output stream
    * in HTML text format.
    * This string will be inserted as is inside a container on the web page,
    * so we must include HTML formatting commands to make things look presentable!
    */
   return 0;
}
