#include "stdafx.h"

struct InfoForExtension
{
	const char* propertyName;
	int propertyValue;
	InfoForExtension* nextProperty;
};

// Traverses linked list to find info.
int GetProperty(const char* propertyName, const InfoForExtension* miscInfo)
{
	const InfoForExtension* miscInfoTraverser = miscInfo;
	while (miscInfoTraverser != nullptr)
		if (strcmp(propertyName, miscInfoTraverser->propertyName) == 0) return miscInfoTraverser->propertyValue;
		else miscInfoTraverser = miscInfoTraverser->nextProperty;

	return 0;
}

extern "C"
{
	/**
	* Param sentence: pointer to sentence received by NextHooker (UTF-16).
	* You should not modify this sentence. If you want NextHooker to receive a modified sentence, copy it into your own buffer and return that.
	* Param miscInfo: pointer to start of singly linked list containing misc info about the sentence.
	* Return value: pointer to sentence NextHooker takes for future processing and display.
	* Return 'sentence' unless you created a new sentence/buffer as mentioned above.
	* NextHooker will display the sentence after all extensions have had a chance to process and/or modify it.
	* THIS FUNCTION MAY BE RUN SEVERAL TIMES CONCURRENTLY: PLEASE ENSURE THAT IT IS THREAD SAFE!
	*/
	__declspec(dllexport) const wchar_t* OnNewSentence(const wchar_t* sentence, const InfoForExtension* miscInfo)
	{
#if 0
		// This example extension places sentences from the hook currently selected by the user into the clipboard
		if (GetProperty("current select", miscInfo))
		{
			HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (wcslen(sentence) + 1) * sizeof(wchar_t));
			memcpy(GlobalLock(hMem), sentence, (wcslen(sentence) + 1) * sizeof(wchar_t));
			GlobalUnlock(hMem);
			OpenClipboard(0);
			EmptyClipboard();
			SetClipboardData(CF_UNICODETEXT, hMem);
			CloseClipboard();
		}
		return sentence; // Return the original since no modifications were made.
#else
		// This example extension adds extra newlines to all sentences
		// Create a new buffer since the sentence needs to be modified. No need to worry about freeing this: NextHooker does it for you.
		// Please allocate the buffer using malloc() and not new[] or something else: NextHooker uses free() to free it.
		wchar_t* newSentence = (wchar_t*)malloc((wcslen(sentence) + 6) * sizeof(wchar_t));
		swprintf(newSentence, wcslen(sentence) + 6, L"%s\r\n", sentence);
		return newSentence;
#endif
	}
}
