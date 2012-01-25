#if !defined(SOLUTIONISOTOPE_H_INCLUDED)
#define SOLUTIONISOTOPE_H_INCLUDED

#include <ostream>				// std::ostream
#include <string>				// std::string
#include <list>					// std::list
#include "Parser.h"

class cxxSolutionIsotope: public PHRQ_base
{
  public:
	cxxSolutionIsotope(PHRQ_io *io=NULL);
	cxxSolutionIsotope(struct isotope *isotope_ptr, PHRQ_io *io=NULL);
	~cxxSolutionIsotope(void);

	void dump_xml(std::ostream & os, unsigned int indent) const;
	void dump_raw(std::ostream & os, unsigned int indent) const;

	CParser::STATUS_TYPE read_raw(CParser & parser, std::istream::pos_type next_char);

	LDBLE Get_isotope_number() const
	{
		return this->isotope_number;
	}
	void Set_isotope_number(LDBLE d)
	{
		this->isotope_number = d;
	}
	const std::string &Get_elt_name() const
	{
		return this->elt_name;
	}
	void Set_elt_name(const char *cstring)
	{
		if (cstring != NULL)
			this->elt_name = std::string(cstring);
		else
			this->elt_name.clear();
	}
	const std::string &Get_isotope_name() const
	{
		return this->isotope_name;
	}
	void Set_isotope_name(const char *cstring)
	{
		if (cstring != NULL)
			this->isotope_name = std::string(cstring);
		else
			this->isotope_name.clear();
	}
	LDBLE Get_total() const
	{
		return this->total;
	}
	void Set_total(LDBLE d)
	{
		this->total = d;
	}

	LDBLE Get_ratio() const
	{
		return this->ratio;
	}

	LDBLE Get_ratio_uncertainty() const
	{
		return this->ratio_uncertainty;
	}

	bool Get_ratio_uncertainty_defined() const
	{
		return this->ratio_uncertainty_defined;
	}

	bool operator<(const cxxSolutionIsotope & conc) const;
	void add(const cxxSolutionIsotope & isotope_ptr, LDBLE intensive,
			 LDBLE extensive);
	void multiply(LDBLE extensive);

  protected:
	friend class cxxSolutionIsotopeList;
	LDBLE isotope_number;
	std::string elt_name;
	std::string isotope_name;
	LDBLE total;
	LDBLE ratio;
	LDBLE ratio_uncertainty;
	bool ratio_uncertainty_defined;
};
#endif // SOLUTIONISOTOPE_H_INCLUDED
