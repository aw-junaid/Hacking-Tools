/*****************************************************************/
// MODULE:	macros.h
// PURPOSES: common helpful macros [DONE]
/*****************************************************************/
#ifndef _MACROS_H
#define _MACROS_H

//expression represent memory (#Pointer# points to,
//at offset of #Offset# bytes) as varibale of #Type#
#define AT_OFFSET(Type,Pointer,Offset) \
	(*((Type*)(((PUCHAR)(Pointer))+(Offset))))

//expression represent pointer to memory (#Pointer# points to,
//at offset of #Offset# bytes) as varibale of #Type#
#define P_OFFSET(Type,Pointer,Offset) \
	((Type)(((PUCHAR)(Pointer))+(Offset)))

//offset of field in struct
#define OFFSET_OF_FIELD(StructType,Field) \
	((ULONG)(&(((StructType*)NULL)->Field)))

//sizeof struct type member
#define SIZE_OF_FIELD(StructType,Field) \
	sizeof(((StructType*)NULL)->Field)

//
//  These macros are used to test, set and clear flags respectivly
//

#ifndef FlagOn
#define FlagOn(_F,_SF)        ((_F) & (_SF))
#endif

#ifndef BooleanFlagOn
#define BooleanFlagOn(F,SF)   ((BOOLEAN)(((F) & (SF)) != 0))
#endif

#ifndef SetFlag
#define SetFlag(_F,_SF)       ((_F) |= (_SF))
#endif

#ifndef ClearFlag
#define ClearFlag(_F,_SF)     ((_F) &= ~(_SF))
#endif

/*
* Rob Green, a member of the NTDEV list, provides the 
* following set of macros that'll keep you from having 
* to scratch your head and count zeros ever again.  
* Using these defintions, all you'll have to do is write:
* 
* interval.QuadPart = RELATIVE(SECONDS(5));
*/

#ifndef ABSOLUTE
#define ABSOLUTE(wait) (wait)
#endif

#ifndef RELATIVE
#define RELATIVE(wait) (-(wait))
#endif

#ifndef NANOSECONDS
#define NANOSECONDS(nanos) \
	(((signed __int64)(nanos)) / 100L)
#endif

#ifndef MICROSECONDS
#define MICROSECONDS(micros) \
	(((signed __int64)(micros)) * NANOSECONDS(1000L))
#endif

#ifndef MILLISECONDS
#define MILLISECONDS(milli) \
	(((signed __int64)(milli)) * MICROSECONDS(1000L))
#endif

#ifndef SECONDS
#define SECONDS(seconds) \
	(((signed __int64)(seconds)) * MILLISECONDS(1000L))
#endif

#endif _MACROS_H