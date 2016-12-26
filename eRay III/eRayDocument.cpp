
#include "eRayDocument.h"

eRayDocument::eRayDocument()
{

}

eRayDocument::~eRayDocument()
{

}

bool eRayDocument::ReadNextString(FILE *f, doc_string *Destination)
{
	size_t tmp_size_t;
	doc_char* Buffer;
	// ��������� ������ ������
	if (fread(&tmp_size_t,sizeof(size_t),1,f) != 1)
		return false;
	// ��������� ������
	Buffer = new doc_char[tmp_size_t];
	if (fread((void*)Buffer,sizeof(doc_char),tmp_size_t,f) != tmp_size_t)
		return false;
	Destination->assign(Buffer,tmp_size_t);
	delete [] Buffer;
	return true;
}

bool eRayDocument::WriteNextString(FILE *f, doc_string *Value)
{
	// �������� ������ ������
	std::size_t tmp_size_t = Value->size();
	// ���������� ������ ������
	if (fwrite(&tmp_size_t,sizeof(size_t),1,f) != 1)
		return false;
	// ���������� ���� ���������
	if (fwrite((void*)Value->c_str(),sizeof(doc_char),tmp_size_t,f) != tmp_size_t)
		return false;
	return true;
}

bool eRayDocument::WriteVarList(FILE *f)
{
	// �������� ���������� ����������
	std::size_t tmp_size_t = var_list.size();
	// ���������� ���������� ����������
	if (fwrite(&tmp_size_t,sizeof(size_t),1,f) != 1)
		return false;
	for (doc_var_list::iterator i = var_list.begin(); i != var_list.end(); i++)
	{
		// ���������� ��� ����������
		if (!WriteNextString(f,&(i->first)))
			return false;
		// ���������� �������� ����������
		if (!WriteNextString(f,&(i->second)))
			return false;
	}
	return true;
}

bool eRayDocument::ReadVarList(FILE *f)
{
	std::size_t tmp_size_t;
	// ��������� ���������� ����������
	if (fread(&tmp_size_t,sizeof(size_t),1,f) != 1)
		return false;
	// ��������� ���������� � ��������
	var_list.resize(tmp_size_t);
	for (std::size_t i = 0; i < var_list.size(); i++)
	{
		// ��������� ��� ����������
		if (!ReadNextString(f,&(var_list[i].first)))
			return false;
		// ��������� �������� ����������
		if (!ReadNextString(f,&(var_list[i].second)))
			return false;
	}
	return true;
}

bool eRayDocument::ReadConfig(FILE *f)
{
	if (fread(&Configuration,sizeof(ConfigParams),1,f) != 1)
		return false;	
	return true;
}

bool eRayDocument::WriteConfig(FILE *f)
{
	if (fwrite(&Configuration,sizeof(ConfigParams),1,f) != 1)
		return false;
	return true;
}

bool eRayDocument::ReadMetalInfo(FILE *f)
{
	doc_string read_string;
	ExpressionError ExprErr;

	if (!ReadNextString(f,&read_string)) return false;
	MetalInformation.m_cLambdaLiq.Initialization(read_string,ExprErr);

	if (!ReadNextString(f,&read_string)) return false;
	MetalInformation.m_cLambdaSol.Initialization(read_string,ExprErr);

	if (!ReadNextString(f,&read_string)) return false;
	MetalInformation.m_cDensityLiq.Initialization(read_string,ExprErr);

	if (!ReadNextString(f,&read_string)) return false;
	MetalInformation.m_cDensitySol.Initialization(read_string,ExprErr);

	if (!ReadNextString(f,&read_string)) return false;
	MetalInformation.m_cTempCapLiq.Initialization(read_string,ExprErr);

	if (!ReadNextString(f,&read_string)) return false;
	MetalInformation.m_cTempCapSol.Initialization(read_string,ExprErr);

	if (fread(&MetalInformation.m_fTemperatureLiq,sizeof(double),1,f) != 1)
		return false;
	if (fread(&MetalInformation.m_fTemperatureSol,sizeof(double),1,f) != 1)
		return false;
	if (fread(&MetalInformation.m_fBlackness,sizeof(double),1,f) != 1)
		return false;
	if (fread(&MetalInformation.m_fFusion,sizeof(double),1,f) != 1)
		return false;
	if (fread(&MetalInformation.m_fViscosity,sizeof(double),1,f) != 1)
		return false;
	if (fread(&MetalInformation.m_fX,sizeof(double),1,f) != 1)
		return false;
	if (fread(&MetalInformation.m_fY,sizeof(double),1,f) != 1)
		return false;
	if (fread(&MetalInformation.m_fZ,sizeof(double),1,f) != 1)
		return false;
	if (fread(&MetalInformation.m_fJ,sizeof(double),1,f) != 1)
		return false;
	if (fread(&MetalInformation.m_fG,sizeof(double),1,f) != 1)
		return false;
	if (fread(&MetalInformation.m_fEfficiency,sizeof(double),1,f) != 1)
		return false;
	return true;
}

bool eRayDocument::WriteMetalInfo(FILE *f)
{
	doc_string write_string;

	write_string = MetalInformation.m_cLambdaLiq.ToString();
	if (!WriteNextString(f,&write_string))
		return false;
	write_string = MetalInformation.m_cLambdaSol.ToString();
	if (!WriteNextString(f,&write_string))
		return false;
	write_string = MetalInformation.m_cDensityLiq.ToString();
	if (!WriteNextString(f,&write_string))
		return false;
	write_string = MetalInformation.m_cDensitySol.ToString();
	if (!WriteNextString(f,&write_string))
		return false;
	write_string = MetalInformation.m_cTempCapLiq.ToString();
	if (!WriteNextString(f,&write_string))
		return false;
	write_string = MetalInformation.m_cTempCapSol.ToString();
	if (!WriteNextString(f,&write_string))
		return false;
	if (fwrite(&MetalInformation.m_fTemperatureLiq,sizeof(double),1,f) != 1)
		return false;
	if (fwrite(&MetalInformation.m_fTemperatureSol,sizeof(double),1,f) != 1)
		return false;
	if (fwrite(&MetalInformation.m_fBlackness,sizeof(double),1,f) != 1)
		return false;
	if (fwrite(&MetalInformation.m_fFusion,sizeof(double),1,f) != 1)
		return false;
	if (fwrite(&MetalInformation.m_fViscosity,sizeof(double),1,f) != 1)
		return false;
	if (fwrite(&MetalInformation.m_fX,sizeof(double),1,f) != 1)
		return false;
	if (fwrite(&MetalInformation.m_fY,sizeof(double),1,f) != 1)
		return false;
	if (fwrite(&MetalInformation.m_fZ,sizeof(double),1,f) != 1)
		return false;
	if (fwrite(&MetalInformation.m_fJ,sizeof(double),1,f) != 1)
		return false;
	if (fwrite(&MetalInformation.m_fG,sizeof(double),1,f) != 1)
		return false;
	if (fwrite(&MetalInformation.m_fEfficiency,sizeof(double),1,f) != 1)
		return false;
	return true;
}
bool eRayDocument::LoadFromFile(const doc_char *FileName)
{
	FILE *file = NULL;
	if (doc_fopen(&file,FileName,DOC_TEXT("r")))
	{
		return false;
	}
	if (file == NULL) return false;

	// ������ ��������� ��� x
	if (!ReadNextString(file,&expr_x))
		goto m_close_error;
	// ������ ��������� ��� y
	if (!ReadNextString(file,&expr_y))
		goto m_close_error;

	//
	// C�������� ����������
	//

	if (!ReadVarList(file))
		goto m_close_error;

	//
	// ��������� ������ � ���������
	//

	// ��������� ������ ���������
	if (!ReadNextString(file,&t_begin))
		goto m_close_error;
	// ��������� ���-�� �����
	if (!ReadNextString(file,&t_stepcount))
		goto m_close_error;
	// ��������� ����� ���������
	if (!ReadNextString(file,&t_end))
		goto m_close_error;

	//
	// ��������� ������ � ����
	//

	// ��������� ������ ����
	if (!ReadNextString(file,&ray_radius))
		goto m_close_error;
	// ��������� ���������� ����
	if (!ReadNextString(file,&ray_voltage))
		goto m_close_error;

	//
	// ��������� ��������� ��������
	//
	
	// ��������� ������ �����������
	if (!ReadNextString(file,&resolution_by_x))
		goto m_close_error;
	// ��������� ������ �����������
	if (!ReadNextString(file,&resolution_by_y))
		goto m_close_error;

	//
	// ��������� ���������� �� ������������� �������
	//

	// ��������� ���� ������������� �������
	if (fread((void*)&use_region,sizeof(use_region),1,file) != 1)
		goto m_close_error;
	// ��������� ������ min x
	if (!ReadNextString(file,&region_min_x))
		goto m_close_error;
	// ��������� ������ min y
	if (!ReadNextString(file,&region_min_y))
		goto m_close_error;
	// ��������� ������ max x
	if (!ReadNextString(file,&region_max_x))
		goto m_close_error;
	// ��������� ������ max y
	if (!ReadNextString(file,&region_max_y))
		goto m_close_error;

	//
	// ��������� ������ ������������
	// 

	if (!ReadConfig(file))
		goto m_close_error;

	if (!ReadMetalInfo(file))
		goto m_close_error;

	fclose(file);
	return true;
m_close_error:
	fclose(file);
	return false;	
}

bool eRayDocument::SaveToFile(const doc_char *FileName)
{
	FILE *file = NULL;
	if (doc_fopen(&file,FileName,DOC_TEXT("w")))
	{
		return false;
	}
	if (file == NULL) return false;

	// ������ ��������� ��� x
	if (!WriteNextString(file,&expr_x))
		goto m_close_error;

	// ������ ��������� ��� y
	if (!WriteNextString(file,&expr_y))
		goto m_close_error;

	//
	// ���������� ������ � ����������
	//

	if (!WriteVarList(file))
		goto m_close_error;

	//
	// ���������� ������ � ���������
	//

	// ���������� ������ ���������
	if (!WriteNextString(file,&t_begin))
		goto m_close_error;
	// ���������� ���-�� �����
	if (!WriteNextString(file,&t_stepcount))
		goto m_close_error;
	// ���������� ����� ���������
	if (!WriteNextString(file,&t_end))
		goto m_close_error;

	//
	// ���������� ������ � ����
	//

	// ���������� ������ ����
	if (!WriteNextString(file,&ray_radius))
		goto m_close_error;
	// ���������� ���������� ����
	if (!WriteNextString(file,&ray_voltage))
		goto m_close_error;

	//
	// ���������� ��������� ��������
	//
	
	// ���������� ������ �����������
	if (!WriteNextString(file,&resolution_by_x))
		goto m_close_error;
	// ���������� ������ �����������
	if (!WriteNextString(file,&resolution_by_y))
		goto m_close_error;

	//
	// ���������� ���������� �� ������������� �������
	//

	// ���������� ���� ������������� �������
	if (fwrite((void*)&use_region,sizeof(use_region),1,file) != 1)
		goto m_close_error;
	// ���������� ������ min x
	if (!WriteNextString(file,&region_min_x))
		goto m_close_error;
	// ���������� ������ min y
	if (!WriteNextString(file,&region_min_y))
		goto m_close_error;
	// ���������� ������ max x
	if (!WriteNextString(file,&region_max_x))
		goto m_close_error;
	// ���������� ������ max y
	if (!WriteNextString(file,&region_max_y))
		goto m_close_error;

	//
	// ���������� ������ ������������
	// 

	if (!WriteConfig(file))
		goto m_close_error;

	if (!WriteMetalInfo(file))
		goto m_close_error;

	fclose(file);
	return true;
m_close_error:
	fclose(file);
	return false;
}

void eRayDocument::set_expr_x(const doc_char *Str)
{
	expr_x.assign(Str);
}
void eRayDocument::set_expr_y(const doc_char *Str)
{
	expr_y.assign(Str);
}
void eRayDocument::set_var_list(doc_var_list *Var_list)
{
	var_list = *(Var_list);
}
void eRayDocument::set_t_begin(const doc_char *Str)
{
	t_begin.assign(Str);
}
void eRayDocument::set_t_stepcount(const doc_char *Str)
{
	t_stepcount.assign(Str);
}
void eRayDocument::set_t_end(const doc_char *Str)
{
	t_end.assign(Str);
}
void eRayDocument::set_ray_radius(const doc_char *Str)
{
	ray_radius.assign(Str);
}
void eRayDocument::set_ray_voltage(const doc_char *Str)
{
	ray_voltage.assign(Str);
}
void eRayDocument::set_resolution_by_x(const doc_char *Str)
{
	resolution_by_x.assign(Str);
}
void eRayDocument::set_resolution_by_y(const doc_char *Str)
{
	resolution_by_y.assign(Str);
}
void eRayDocument::set_use_region(bool use_state)
{
	use_region = use_state;
}
void eRayDocument::set_region_min_x(const doc_char *Str)
{
	region_min_x.assign(Str);
}
void eRayDocument::set_region_min_y(const doc_char *Str)
{
	region_min_y.assign(Str);
}
void eRayDocument::set_region_max_x(const doc_char *Str)
{
	region_max_x.assign(Str);
}
void eRayDocument::set_region_max_y(const doc_char *Str)
{
	region_max_y.assign(Str);
}
void eRayDocument::set_configuration(ConfigParams *Config)
{
	Configuration = *Config;
}

void eRayDocument::set_metalinfo(CMetalInfo *Config)
{
	MetalInformation = *Config;
}

doc_string eRayDocument::get_expr_x()
{
	return expr_x;
}
doc_string eRayDocument::get_expr_y()
{
	return expr_y;
}
void eRayDocument::get_var_list(doc_var_list *Var_list)
{
	*(Var_list) = var_list;
}
doc_string eRayDocument::get_t_begin()
{
	return t_begin;
}
doc_string eRayDocument::get_t_stepcount()
{
	return t_stepcount;
}
doc_string eRayDocument::get_t_end()
{
	return t_end;
}
doc_string eRayDocument::get_ray_radius()
{
	return ray_radius;
}
doc_string eRayDocument::get_ray_voltage()
{
	return ray_voltage;
}
doc_string eRayDocument::get_resolution_by_x()
{
	return resolution_by_x;
}
doc_string eRayDocument::get_resolution_by_y()
{
	return resolution_by_y;
}
bool eRayDocument::get_use_region()
{
	return use_region;
}
doc_string eRayDocument::get_region_min_x()
{
	return region_min_x;
}
doc_string eRayDocument::get_region_min_y()
{
	return region_min_y;
}
doc_string eRayDocument::get_region_max_x()
{
	return region_max_x;
}
doc_string eRayDocument::get_region_max_y()
{
	return region_max_y;
}
void eRayDocument::get_configuration(ConfigParams *Config)
{
	*Config = Configuration;
}

void eRayDocument::get_metalinfo(CMetalInfo *Config)
{
	*Config = MetalInformation;
}