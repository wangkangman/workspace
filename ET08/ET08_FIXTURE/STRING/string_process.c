#include "string_process.h"
#include "stdlib.h"

static u8* GetSectionPos(u8 *text, u8 *section);


u8 GetSectionVal(u8 *text, u8 *section, u8 *key, u8 *val) {
  u8* section_text;
  u8 *temp_key;
  u8 *temp_text;
  section_text = GetSectionPos(text, section);
  if ((void*)0 == section_text) return 1;
  
  while (1) {
    if ('\0' == *section_text || '[' == *section_text) {
      return 2;
    }

    temp_text = section_text;
    temp_key = key;
    while (*temp_text == *temp_key) {
      temp_text++;
      temp_key++;
      if ('\0' == *temp_key)
        goto SUCCESS;
    }

    section_text++;
  }

SUCCESS:
  while (*section_text++ != '=') {
    if (*section_text == '\r' 
       || *section_text == '\n' 
       || *section_text == '\0' 
       || *section_text == '[')
      return 3;
  }

  while (*section_text == ' ') {
    section_text++;
  }

  while (*section_text != '\r'
         && *section_text != '\n'
         && *section_text != '\0'
         && *section_text != ' '
         && *section_text != '[') {
    *val++ = *section_text++;
  }

  *val = '\0';

  return 0;
}



u8* GetSectionPos(u8 *text, u8 *section) {
  u8 *temp_key;
  u8 *temp_text;

  temp_key = section;
  temp_text = text;

  while (1) {
    while ('[' != *text++)
      if ('\0' == *text) return (void*)0;


    temp_text = text;
    temp_key = section;
    while (*temp_text == *temp_key) {
      temp_text++;
      temp_key++;
      if ('\0' == *temp_key)
        goto SUCCESS;
    }
  }

SUCCESS:
  while ('\n' != *text++){}

  return text;
}



u8 GetPrivateString(u8 *text, u8 *key, u8 *val) {
  u8 *temp_key;
  u8 *temp_text;

  temp_key = key;
  temp_text = text;

  while (1) {
    if ('\0' == *text) {
      return 1;
    }

    temp_text = text;
    temp_key = key;
    while (*temp_text == *temp_key) {
      temp_text++;
      temp_key++;
      if ('\0' == *temp_key)
        goto SUCCESS;
    }

    text++;
  }

SUCCESS:
  while (*text++ != '=') {
    if (*text == '\r' || *text == '\n' || *text == '\0')
      return 2;
  }

  while (*text == ' ') {
    text++;
  }

  while (*text != '\r' && *text != '\n' && *text != '\0' && *text != ' ') {
    *val++ = *text++;
  }
  *val = '\0';

  return 0;
}


u8 GetLimitVal(u8 *str, float* low, float* upper){
  u8 val[10];
  u8* p_val = &val[0];
	
  while(*str != '~') {
    *p_val++ = *str++;
    if ('\0' == *str) return 1;
  }

  *p_val = '\0';

  *low = atof((char*)val);

	
  p_val = val;
	str++;
  while(*str != '\0') {
    *p_val++ = *str++;
  }
  *p_val = '\0';

  *upper = atof((char*)val);

  return 0;
}

