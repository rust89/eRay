#ifndef _ERAY_DOCUMENT_H
#define _ERAY_DOCUMENT_H

#include <string>
#include <vector>

#include "RModelParamFrame.h"

// типы данных

#ifdef UNICODE
	typedef std::wstring doc_string;
	typedef wchar_t doc_char;
	#define __DOC_TEXT(quote) L##quote
#else
	typedef std::string doc_string;
	typedef char doc_char;
	#define __DOC_TEXT(quote) quote
#endif

#define DOC_TEXT(quote) __DOC_TEXT(quote)

typedef std::vector<std::pair<doc_string,doc_string>> doc_var_list;

// кодировка - зависимые функции
#ifdef UNICODE
	#define doc_fopen _wfopen_s
#else
	#define doc_fopen fopen_s
#endif

enum e3DOption
{
	e3doLow = 1, 
	e3doNorm = 2, 
	e3doHigh = 3 
};

struct ConfigParams
{
	// общие настройки
	bool СommonOptimiz;
	bool CommonAccRayRad;
	e3DOption Option3D;
	// классический график
	bool ClassicDrawAxes;
	bool ClassicDrawNode;
	bool ClassicDrawBound;
	// градиентный график
	bool GCloudDrawAxes;
	bool GCloudDrawBound;
	bool GCloudDrawNodeGrid;
	int GCloudNodeGridStep;
	// радужный график
	bool NGCloudDrawAxes;
	bool NGCloudDrawBound;
	bool NGCloudeDrawScale;
};

class eRayDocument
{
	protected:
		doc_string expr_x;
		doc_string expr_y;

		doc_var_list var_list;

		doc_string t_begin;
		doc_string t_stepcount;
		doc_string t_end;

		doc_string ray_radius;
		doc_string ray_voltage;

		doc_string resolution_by_x;
		doc_string resolution_by_y;

		bool use_region;
		doc_string region_min_x;
		doc_string region_min_y;
		doc_string region_max_x;
		doc_string region_max_y;

		ConfigParams Configuration;
		CMetalInfo MetalInformation;

		bool ReadNextString(FILE *f, doc_string *Destination);
		bool WriteNextString(FILE *f, doc_string *Value);

		bool WriteVarList(FILE *f);
		bool ReadVarList(FILE *f);

		bool ReadConfig(FILE *f);
		bool WriteConfig(FILE *f);

		bool ReadMetalInfo(FILE *f);
		bool WriteMetalInfo(FILE *f);

	public:
		eRayDocument();
		~eRayDocument();

		bool LoadFromFile(const doc_char *FileName);
		bool SaveToFile(const doc_char *FileName);

		void set_expr_x(const doc_char *Str);
		void set_expr_y(const doc_char *Str);
		void set_var_list(doc_var_list *Var_list);
		void set_t_begin(const doc_char *Str);
		void set_t_stepcount(const doc_char *Str);
		void set_t_end(const doc_char *Str);
		void set_ray_radius(const doc_char *Str);
		void set_ray_voltage(const doc_char *Str);
		void set_resolution_by_x(const doc_char *Str);
		void set_resolution_by_y(const doc_char *Str);
		void set_use_region(bool use_state);
		void set_region_min_x(const doc_char *Str);
		void set_region_min_y(const doc_char *Str);
		void set_region_max_x(const doc_char *Str);
		void set_region_max_y(const doc_char *Str);
		void set_configuration(ConfigParams *Config);
		void set_metalinfo(CMetalInfo *Config);


		doc_string get_expr_x();
		doc_string get_expr_y();
		void get_var_list(doc_var_list *Var_list);
		doc_string get_t_begin();
		doc_string get_t_stepcount();
		doc_string get_t_end();
		doc_string get_ray_radius();
		doc_string get_ray_voltage();
		doc_string get_resolution_by_x();
		doc_string get_resolution_by_y();
		bool get_use_region();
		doc_string get_region_min_x();
		doc_string get_region_min_y();
		doc_string get_region_max_x();
		doc_string get_region_max_y();
		void get_configuration(ConfigParams *Config);
		void get_metalinfo(CMetalInfo *Config);
};

#endif