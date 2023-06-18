#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>

// The input to this program might look like:
// ./program thunderbird-31.3.0.source.tar.bz2
// assuming that you have such a file on your computer.  Your
// input can be any previously compressed file you want to
// test predict.

// The concept here is that this program doesn't require access
// to the input, the program predicts THE APPROXIMATE VALUE of
// each next byte in the compressed file you've supplied, never
// looking at the file.

// This program is intended as a demonstration of new technology.
// That's all I claim for this code.

// At one time (year's ago,) I had an interest in repeated
// compression, not-so today.  These days, for me, it's about
// using this technology (eg., a program based on this method,)
// to pass messages backward in time.

// This program is driven by a pseudo-random-number-generator,
// (a 'PRNG'), but to recover simple messages not-yet-sent I
// use a true-random-number-generator, (or 'TRNG') with another
// program, an EXPERIMENTAL program.  Since 2015 or so, Intel's
// x86 line has included a platform for producing true random
// data using quantum methods, which my program depends on. 

// Also, people tend to assume that the SEND-side and RCVE-side
// are simply PC's with a message that exists before it's sent.
// Not always.  This demondtration program, while just a toy,
// shows that one can guess with 75% success the approximate
// value of an unread byte in a compressed file, even though the
// sought file (possibly,) hasn't been created yet.

// This code is pretty simple.  And yes it does read the client's
// unsent message but only to validate the success merit.

/////////////////////////////////////////////////////////////////////////////

#define rdm8()    ((random() >> 12) & 255)

enum { LT=0, GE };

/////////////////////////////////////////////////////////////////////////////

FILE *iFILE;

int nCHAR;

unsigned char  raw;

int  newSR, oldSD, oldSRoldSD, newSRoldSD;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

int
mimicGE()
{
    // this predicts whether the next byte
    // in some unseen file containing previously
    // compressed data, and meets:
    //
    //            r >= d
    //
    // where 'r' is the value of a raw 8-bit random
    // number, and 'd' is the next value in a vector
    // from a previously compressed data vector.
    //
    // Both 'r' and 'd' are unsigned char's.
    //
    // In ah, real-life it's important to use random
    // data from nature, not pseudo-random data as this
    // uses.
    //
    // This does something very simple, it predicts
    // the "r >= d" relationship with better than 75%
    // accuracy, without examining the 'd' data value.
    //
    // Other systems predict bits, (with the same
    // merit,) this is a demo, I stop at the byte level.
    //
    // Recovered (or predicted,) data can be on the
    // same computer or another computer.
    //
    // This program make it's predictions without regard
    // to past or future guesses or observations.
    //
    // In fact, the client file doesn't have to exist
    // at the time the prediction is framed.
    //
    // And please don't say this can't possibly work.
    // Just run this simple program, you'll see that it
    // works quite well.
    //
    // NOTE1:  I do not promise useful results with
    // non-random data, this program will not work well
    // with, say, document data unless it has been XOR'ed
    // random content (by keeping track of the seed one
    // can restore the garbled byte values.

    if ((raw <= 119)) return 0;
    if ((raw <= 128) && (newSR >= 106)) return 0;
    if ((raw <= 128) && (raw >= 124) && (oldSD >= -35))
        return 0;
    if ((raw <= 134) && (oldSRoldSD >= -29) && (newSRoldSD <= 100))
        return 0;
    if ((raw <= 143) && (newSRoldSD >= 108) && (newSR <= 88) &&
        (newSR >= 83) && (oldSRoldSD >= 8)) return 0;
    return 1;
}

/////////////////////////////////////////////////////////////////////////////

long
sizeofFILE( FILE *f)
{
    long  current, filesize;
    current = ftell(f);
    fseek(f, 0L, SEEK_END);
    filesize = ftell(f);
    fseek(f, current, SEEK_SET);
    rewind(f);
    return filesize;
}

/////////////////////////////////////////////////////////////////////////////

void
main(int argc, char *argv[])
{
    int k,i;
    int guessWasWrong = 0;
    int guessWasRight = 0;

    int sr = 0;
    int sd = 0;

    if (argc == 1) printf("program testfile\n");
    if (argc != 2) exit(0);

    iFILE = fopen(argv[1], "rb");
    nCHAR = sizeofFILE(iFILE);

    for(i=0;i<nCHAR;i++)
    {
        unsigned char jaz = rdm8();
        unsigned char dat = jaz ^ fgetc(iFILE);

        int actFLAG, prdFLAG, residue;

        raw = rdm8();  // 'raw' is a global variable

        oldSD = sd;        oldSRoldSD = sr - sd;
        sr += raw;         newSRoldSD = sr - sd;
        newSR = sr;

        actFLAG = raw >= dat;
        prdFLAG = mimicGE();

        residue = (prdFLAG == 1) ? ( (signed)raw - (signed)dat ) :
                  ( (signed)dat - (signed)raw );
        if (residue < 0) guessWasWrong++;
        // else printf("%d %+06d\n", prdFLAG , residue );

        // 'actFLAG' is the true value,
        // 'prdFLAG' is the predicted value, so
        // of course 00 and 11 values are fine.
        // Uncomment this next line to see these.
        // This demo is right 75% using gzipped 
        // compressed files which doesn't have to
        // exist at the time the file is guessed.

        // printf("%d%d\n",actFLAG,prdFLAG);
        if (actFLAG == prdFLAG) ++guessWasRight;

        sd += rdm8();
        sr /= -2;
        sd /= -2;
    }

    fclose(iFILE);

    // show single-pass merit, typically about 75%.
    printf("%6.3f%%  %6.3f%%  %d\n" , (100. * guessWasWrong) / nCHAR ,
           (100. * guessWasRight) / nCHAR , nCHAR);
}

/////////////////////////////////////////////////////////////////////////////