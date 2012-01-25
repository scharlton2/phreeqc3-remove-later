// NameDouble.cxx: implementation of the cxxNameDouble class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include <cassert>				// assert
#include <algorithm>			// std::sort
#include <map>					// std::sort
#include <iostream>				// std::cout std::cerr

#include "Utils.h"				// define first
#include "Phreeqc.h"
#include "NameDouble.h"
//#include "Dictionary.h"
#include "phqalloc.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxNameDouble::cxxNameDouble()
	//
	// default constructor for cxxNameDouble 
	//
{
}

cxxNameDouble::cxxNameDouble(struct elt_list *elt_list_ptr)
		//
		// constructor for cxxNameDouble from list of elt_list
		//
{
	int i;
	if (elt_list_ptr != NULL)
	{
		for (i = 0; elt_list_ptr[i].elt != NULL; i++)
		{
			(*this)[elt_list_ptr[i].elt->name] = elt_list_ptr[i].coef;
		}
	}
	this->type = ND_ELT_MOLES;
}

cxxNameDouble::cxxNameDouble(struct elt_list *elt_list_ptr, int count)
		//
		// constructor for cxxNameDouble from list of elt_list with known count
		//
{
	int i;
	if (elt_list_ptr != NULL)
	{
		for (i = 0; i < count; i++)
		{
			(*this)[elt_list_ptr[i].elt->name] = elt_list_ptr[i].coef;
		}
	}
	this->type = ND_ELT_MOLES;
}

cxxNameDouble::cxxNameDouble(const cxxNameDouble & old, LDBLE factor)
		//
		// constructor for cxxNameDouble from list of elt_list
		//
{
	for (cxxNameDouble::const_iterator it = old.begin(); it != old.end();
		 it++)
	{
		if (old.type == ND_ELT_MOLES)
		{
			if (it->second * factor > 0)
			{
				(*this)[(it->first)] = it->second * factor;
			}
		}
		else
		{
			(*this)[(it->first)] = it->second * factor;
		}
	}
	this->type = old.type;
}
cxxNameDouble::cxxNameDouble(struct conc *tots)
		//
		// constructor for cxxNameDouble from list of elt_list
		//
{
	int i;
	for (i = 0; tots[i].description != NULL; i++)
	{
		(*this)[tots[i].description] = tots[i].moles;
	}
	this->type = ND_ELT_MOLES;
}
cxxNameDouble::cxxNameDouble(struct master_activity *ma, int count,
							 cxxNameDouble::ND_TYPE l_type)
		//
		// constructor for cxxNameDouble from list of elt_list
		//
{
	int i;
	for (i = 0; i < count; i++)
	{
		if (ma[i].description == NULL)
			continue;
		(*this)[ma[i].description] = ma[i].la;
	}
	this->type = l_type;
}
/*
cxxNameDouble::cxxNameDouble(struct name_coef *nc, int count)
		//
		// constructor for cxxNameDouble from list of elt_list
		//
{
	int i;
	for (i = 0; i < count; i++)
	{
		if (nc[i].name == NULL)
			continue;
		(*this)[nc[i].name] = nc[i].coef;
	}
	this->type = ND_NAME_COEF;
}
*/
cxxNameDouble::cxxNameDouble(struct name_coef *nc, int count)
		//
		// constructor for cxxNameDouble from list of elt_list
		//
{
	int i;
	for (i = 0; i < count; i++)
	{
		if (nc[i].name == NULL)
			continue;

		if ((*this).find(nc[i].name) == (*this).end())
		{
			(*this)[nc[i].name] = nc[i].coef;
		}
		else
		{
			(*this)[nc[i].name] = (*this).find(nc[i].name)->second + nc[i].coef;
		}

	}
	this->type = ND_NAME_COEF;
}

cxxNameDouble::~cxxNameDouble()
{
}

void
cxxNameDouble::dump_xml(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing exch_comp message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	std::string xmlElement, xmlAtt1, xmlAtt2;

	switch ((*this).type)
	{
	case cxxNameDouble::ND_SPECIES_LA:
		xmlElement = "<soln_m_a ";
		xmlAtt1 = " m_a_desc=\"";
		xmlAtt1 = " m_a_la=\"";
		break;
	case cxxNameDouble::ND_SPECIES_GAMMA:
		xmlElement = "<soln_s_g ";
		xmlAtt1 = " m_a_desc=\"";
		xmlAtt1 = " m_a_la=\"";
		break;
	case cxxNameDouble::ND_ELT_MOLES:
		xmlElement = "<soln_total ";
		xmlAtt1 = " conc_desc=\"";
		xmlAtt1 = " conc_moles=\"";
		break;
	case cxxNameDouble::ND_NAME_COEF:
		xmlElement = "<NameCoef ";
		xmlAtt1 = " name=\"";
		xmlAtt1 = " coef=\"";
		break;
	}

	for (const_iterator it = (*this).begin(); it != (*this).end(); it++)
	{
		s_oss << indent0;
		s_oss << xmlElement << xmlAtt1 << it->first << xmlAtt2 << it->
			second << "/>" << "\n";
	}
}

void
cxxNameDouble::dump_raw(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing exch_comp message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);

	for (const_iterator it = (*this).begin(); it != (*this).end(); it++)
	{
		s_oss << indent0;
		s_oss << it->first << "   " << it->second << "\n";
	}
}

CParser::STATUS_TYPE cxxNameDouble::read_raw(CParser & parser,
											 std::istream::pos_type & pos)
{
	std::string token;
	//char *
	//	ctoken;
	LDBLE
		d;

	CParser::TOKEN_TYPE j;

	//m_line_iss.seekg(pos);        

	j = parser.copy_token(token, pos);

	if (j == CParser::TT_EMPTY)
		return CParser::PARSER_OK;

	if (!(parser.get_iss() >> d))
	{
		return CParser::PARSER_ERROR;
	}
	(*this)[token.c_str()] = d;
	return CParser::PARSER_OK;
}

void
cxxNameDouble::add_extensive(const cxxNameDouble & addee, LDBLE factor)
//
// Sums two name doubles, this + factor*nd2
//
{
	if (factor == 0)
		return;
	//assert(factor > 0.0);
	for (cxxNameDouble::const_iterator it = addee.begin(); it != addee.end();
		 it++)
	{
		cxxNameDouble::iterator current = (*this).find(it->first);
		if (current != (*this).end())
		{
			(*this)[it->first] = current->second + it->second * factor;
		}
		else
		{
			(*this)[it->first] = it->second * factor;
		}
	}
}
void
cxxNameDouble::add_intensive(const cxxNameDouble & addee, LDBLE f1,
							 LDBLE f2)
//
// Sums two name doubles, this*f1 + f2*nd2
//
{
	assert(f1 >= 0 && f2 >= 0);
	for (cxxNameDouble::const_iterator it = addee.begin(); it != addee.end();
		 it++)
	{
		cxxNameDouble::iterator current = (*this).find(it->first);
		if (current != (*this).end())
		{
			(*this)[it->first] = f1 * current->second + f2 * it->second;
		}
		else
		{
			(*this)[it->first] = f2 * it->second;
		}
	}
}
void
cxxNameDouble::add_log_activities(const cxxNameDouble & addee, LDBLE f1,
								  LDBLE f2)
//
// Sums two name doubles, this*f1 + f2*nd2, assuming log values
//
{
	assert(f1 >= 0 && f2 >= 0);
	for (cxxNameDouble::const_iterator it = addee.begin(); it != addee.end();
		 it++)
	{
		cxxNameDouble::iterator current = (*this).find(it->first);
		if (current != (*this).end())
		{
			LDBLE a1 = pow((LDBLE) 10., current->second);
			LDBLE a2 = pow((LDBLE) 10., it->second);
			(*this)[it->first] = log10(f1 * a1 + f2 * a2);
		}
		else
		{
			//LDBLE a2 = pow(10. it->second);
			//(*this)[it->first] = log10(f2 * a2);
			(*this)[it->first] = it->second + log10(f2);
		}
	}
}

void
cxxNameDouble::add(const char *token, LDBLE total)
//
// add to total for a specified element
//
{
	char key[MAX_LENGTH];
	strcpy(key, token);

	cxxNameDouble::iterator current = (*this).find(key);
	if (current != (*this).end())
	{
		(*this)[key] = current->second + total;
	}
	else
	{
		(*this)[key] = total;
	}
}
void
cxxNameDouble::multiply(LDBLE extensive)
{
//
// Multiplies by extensive
//

	for (cxxNameDouble::iterator it = this->begin(); it != this->end(); it++)
	{
		it->second *= extensive;
	}
}
void
cxxNameDouble::merge_redox(const cxxNameDouble & source)
//
// Merges source into this
// Accounts for possible conflicts between redox state and
// totals
//
{
	for (cxxNameDouble::const_iterator sit = source.begin(); sit != source.end(); sit++)
	{
		
		std::string redox_name = sit->first;
		std::string elt_name;
		size_t pos = redox_name.find("(");

		bool redox;
		if (pos != std::string::npos) 
		{
			redox = true;
			elt_name = redox_name.substr(0, pos - 1);
		}
		else
		{
			redox = false;
			elt_name = redox_name;
		}
		if (redox)
		{
			// Remove elt_name, if present
			if ((*this).find(elt_name) != (*this).end())
			{
				(*this).erase((*this).find(elt_name));
			}
			// Put in redox name
			(*this)[redox_name] = sit->second;
		}
		else
		{
			std::string substring;
			substring.append(elt_name);
			substring.append("(");

			// Remove all redox
			bool deleted = true;
			while (deleted)
			{
				deleted = false;
				cxxNameDouble::iterator current = (*this).begin();
				for ( ; current != (*this).end(); current++)
				{
					if (current->first.find(substring) == 0)
					{
						(*this).erase(current);
						deleted = true;
						break;
					}
				}
			}
			// Put in elt name
			(*this)[elt_name] = sit->second;
		}

	}
}
struct DblCmp {     
	bool operator()(const std::pair<std::string, LDBLE> &lhs, const std::pair<std::string, LDBLE> &rhs) 
	{         
		return lhs.second > rhs.second;     
	} 
}; 
std::vector< std::pair<std::string, LDBLE> > 
cxxNameDouble::sort_second(void)
{
	std::vector< std::pair<std::string, LDBLE> > myvec(this->begin(), this->end()); 
	std::sort(myvec.begin(), myvec.end(), DblCmp());

	return myvec;
}
#ifdef USE_MPI
void
cxxNameDouble::mpi_pack(std::vector < int >&ints,
						std::vector < LDBLE >&doubles)
{
	extern cxxDictionary dictionary;
	ints.push_back((int) (*this).size());
	for (const_iterator it = (*this).begin(); it != (*this).end(); it++)
	{
		int n = dictionary.string2int(it->first);
		ints.push_back(n);
		doubles.push_back(it->second);
	}
}
void
cxxNameDouble::mpi_pack(int *ints, int *ii, LDBLE *doubles, int *dd)
{
	int i = *ii;
	int d = *dd;
	extern cxxDictionary dictionary;
	//ints.push_back( (*this).size() );
	ints[i++] = (int) this->size();
	for (const_iterator it = this->begin(); it != this->end(); it++)
	{
		int n = dictionary.string2int(it->first);
		if (n < 0)
		{
			std::cerr << it->first << "\n";
			error_msg("Name in NameDouble was not defined in dictionary?\n",
					  STOP);
		}
		//ints.push_back(n);
		ints[i++] = n;
		//doubles.push_back(it->second);
		doubles[d++] = it->second;
	}
	*ii = i;
	*dd = d;
}

//void
//cxxNameDouble::mpi_unpack(int *ints, int *ii, LDBLE *doubles, int *dd)
//{
//	int i = *ii;
//	int d = *dd;
//	extern cxxDictionary dictionary;
//	this->clear();
//	int count = ints[i++];
//	for (int j = 0; j < count; j++)
//	{
//		int n = ints[i++];
//		assert(n >= 0);
//		std::string * str = dictionary.int2string(n);
//		if (str != NULL)
//		{
//			char *cstr = string_hsave(str->c_str());
//			(*this)[cstr] = doubles[d++];
//		}
//	}
//	*ii = i;
//	*dd = d;
//}
void
cxxNameDouble::mpi_unpack(int *ints, int *ii, LDBLE *doubles, int *dd)
{
	int i = *ii;
	int d = *dd;
	extern cxxDictionary dictionary;
	this->clear();
	int count = ints[i++];
	for (int j = 0; j < count; j++)
	{
		int n = ints[i++];
		assert(n >= 0);
		std::string str = dictionary.int2stdstring(n);
		if (str.size() != 0)
		{
			(*this)[str] = doubles[d++];
		}
	}
	*ii = i;
	*dd = d;
}
#endif
