// PPassemblageComp.cxx: implementation of the cxxPPassemblageComp class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include <cassert>				// assert
#include <algorithm>			// std::sort

#include "Utils.h"				// define first
#include "Phreeqc.h"
#include "PPassemblageComp.h"
//#include "Dictionary.h"
#include "phqalloc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxPPassemblageComp::cxxPPassemblageComp(PHRQ_io *io)
:
PHRQ_base(io)
	//
	// default constructor for cxxPPassemblageComp 
	//
{
	si = 0;
	si_org = 0;
	moles = 10;
	delta = 0;
	initial_moles = 0;
	force_equality = false;
	dissolve_only = false;
	precipitate_only = false;
}

cxxPPassemblageComp::~cxxPPassemblageComp()
{
}

void
cxxPPassemblageComp::dump_xml(std::ostream & s_oss, unsigned int indent) const
{
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// Pure_Phase element and attributes

	s_oss << indent0 << "name=\"" << this->name << "\"" << "\n";
	s_oss << indent0 << "add_formula=\"" << this->add_formula << "\"" << "\n";
	s_oss << indent0 << "si=\"" << this->si << "\"" << "\n";
	s_oss << indent0 << "si_org=\"" << this->si_org << "\"" << "\n";
	s_oss << indent0 << "moles=\"" << this->moles << "\"" << "\n";
	s_oss << indent0 << "delta=\"" << this->delta << "\"" << "\n";
	s_oss << indent0 << "initial_moles=\"" << this->
		initial_moles << "\"" << "\n";
	s_oss << indent0 << "force_equality=\"" << this->
		force_equality << "\"" << "\n";
	s_oss << indent0 << "dissolve_only=\"" << this->
		dissolve_only << "\"" << "\n";
	s_oss << indent0 << "precipitate_only=\"" << this->
		precipitate_only << "\"" << "\n";

}

void
cxxPPassemblageComp::dump_raw(std::ostream & s_oss, unsigned int indent) const
{
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// Pure_Phase element and attributes
	s_oss << indent1 << "# EQUILIBRIUM_PHASES_MODIFY candidate identifiers #\n";
	if (this->add_formula.size() != 0)
		s_oss << indent1 << "-add_formula           " << this->add_formula << "\n";
	s_oss << indent1 << "-si                    " << this->si << "\n";
	s_oss << indent1 << "-moles                 " << this->moles << "\n";
	s_oss << indent1 << "-force_equality        " << this->force_equality << "\n";
	s_oss << indent1 << "-dissolve_only         " << this->dissolve_only << "\n";
	s_oss << indent1 << "-precipitate_only      " << this->precipitate_only << "\n";

	s_oss << indent1 << "# PPassemblage workspace variables #\n";
	s_oss << indent1 << "-si_org                " << this->si_org << "\n";
	s_oss << indent1 << "-delta                 " << this->delta << "\n";
	s_oss << indent1 << "-initial_moles         " << this->initial_moles << "\n";
	s_oss << indent1 << "-totals                " << "\n";
	this->totals.dump_raw(s_oss, indent + 2);
}

void
cxxPPassemblageComp::read_raw(CParser & parser, bool check)
{
	std::string str;

	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(10);
		vopts.push_back("name");	// 0                 
		vopts.push_back("add_formula");	// 1
		vopts.push_back("si");	// 2
		vopts.push_back("moles");	// 3
		vopts.push_back("delta");	// 4
		vopts.push_back("initial_moles");	// 5     
		vopts.push_back("dissolve_only");	// 6
		vopts.push_back("force_equality");	// 7
		vopts.push_back("precipitate_only");	// 8
		vopts.push_back("si_org");	// 9
		vopts.push_back("totals");	// 10
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;

	opt_save = CParser::OPT_ERROR;
	bool si_defined(false);
	bool si_org_defined(false);
	bool moles_defined(false);
	bool delta_defined(false);
	bool initial_moles_defined(false);
	bool dissolve_only_defined(false);
	bool force_equality_defined(false);
	bool precipitate_only_defined(false);

	for (;;)
	{
		int opt = parser.get_option(vopts, next_char);
		if (opt == CParser::OPT_DEFAULT)
		{
			opt = opt_save;
		}

		switch (opt)
		{
		case CParser::OPT_EOF:
			break;
		case CParser::OPT_KEYWORD:
			break;
		case CParser::OPT_DEFAULT:
		case CParser::OPT_ERROR:
			opt = CParser::OPT_KEYWORD;
			// Allow return to Exchange for more processing
			break;

		case 0:				// name
			parser.warning_msg("-name ignored. Name is defined with -component.");
			break;

		case 1:				// add_formula
			if (!(parser.get_iss() >> str))
			{
				this->add_formula.clear();
				parser.incr_input_error();
				parser.error_msg("Expected string value for add_formula.",
								 PHRQ_io::OT_CONTINUE);
			}
			else
			{
				this->add_formula = str;
			}
			break;

		case 2:				// si
			if (!(parser.get_iss() >> this->si))
			{
				this->si = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for si.",
								 PHRQ_io::OT_CONTINUE);
			}
			si_defined = true;
			break;

		case 3:				// moles
			if (!(parser.get_iss() >> this->moles))
			{
				this->moles = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for moles.",
								 PHRQ_io::OT_CONTINUE);
			}
			moles_defined = true;
			break;

		case 4:				// delta
			if (!(parser.get_iss() >> this->delta))
			{
				this->delta = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for delta.",
								 PHRQ_io::OT_CONTINUE);
			}
			delta_defined = true;
			break;

		case 5:				// initial_moles
			if (!(parser.get_iss() >> this->initial_moles))
			{
				this->initial_moles = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for initial_moles.",
								 PHRQ_io::OT_CONTINUE);
			}
			initial_moles_defined = true;
			break;


		case 6:				// dissolve_only
			if (!(parser.get_iss() >> this->dissolve_only))
			{
				this->dissolve_only = false;
				parser.incr_input_error();
				parser.error_msg("Expected boolean value for dissolve_only.",
								 PHRQ_io::OT_CONTINUE);
			}
			dissolve_only_defined = true;
			if (this->dissolve_only)
			{
				this->precipitate_only = false;
			}
			break;

		case 7:				// force_equality
			if (!(parser.get_iss() >> this->force_equality))
			{
				this->force_equality = false;
				parser.incr_input_error();
				parser.error_msg("Expected boolean value for force_equality.",
								 PHRQ_io::OT_CONTINUE);
			}
			force_equality_defined = true;
			break;

		case 8:				// precipitate_only
			if (!(parser.get_iss() >> this->precipitate_only))
			{
				this->precipitate_only = false;
				parser.incr_input_error();
				parser.error_msg("Expected boolean value for precipitate_only.",
								 PHRQ_io::OT_CONTINUE);
			}
			precipitate_only_defined = true;
			if (this->precipitate_only)
			{
				this->dissolve_only = false;
			}
			break;
		case 9:				// si_org
			if (!(parser.get_iss() >> this->si_org))
			{
				this->si_org = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for si_org.",
								 PHRQ_io::OT_CONTINUE);
			}
			si_org_defined = true;
			break;
		case 10:				// totals
			if (this->totals.read_raw(parser, next_char) !=	CParser::PARSER_OK)
			{
				parser.incr_input_error();
				parser.
					error_msg
					("Expected element name and molality for Surface totals.",
					 PHRQ_io::OT_CONTINUE);
			}
			opt_save = 10;
			break;
		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	// members that must be defined
	if (check)
	{
		if (si_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Si not defined for PPassemblageComp input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (moles_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Moles not defined for PPassemblageComp input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (delta_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Delta not defined for PPassemblageComp input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (initial_moles_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Initial_moles not defined for PPassemblageComp input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (dissolve_only_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Dissolve_only not defined for PPassemblageComp input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (force_equality_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg
				("Force_equality not defined for PPassemblageComp input.",
				PHRQ_io::OT_CONTINUE);
		}
	}
}

#ifdef USE_MPI
void
cxxPPassemblageComp::mpi_pack(std::vector < int >&ints,
							  std::vector < LDBLE >&doubles)
{
	extern cxxDictionary dictionary;

	ints.push_back(dictionary.string2int(this->name));
	ints.push_back(dictionary.string2int(this->add_formula));
	doubles.push_back(this->si);
	doubles.push_back(this->si_org);
	doubles.push_back(this->moles);
	doubles.push_back(this->delta);
	doubles.push_back(this->initial_moles);
	ints.push_back((int) this->force_equality);
	ints.push_back((int) this->dissolve_only);
	ints.push_back((int) this->precipitate_only);
}

void
cxxPPassemblageComp::mpi_unpack(int *ints, int *ii, LDBLE *doubles, int *dd)
{
	extern cxxDictionary dictionary;
	int i = *ii;
	int d = *dd;
	this->name = dictionary.int2stdstring(ints[i++]);
	this->add_formula = dictionary.int2stdstring(ints[i++]);
	this->si = doubles[d++];
	this->si_org = doubles[d++];
	this->moles = doubles[d++];
	this->delta = doubles[d++];
	this->initial_moles = doubles[d++];
	this->force_equality = (ints[i++] != 0);
	this->dissolve_only = (ints[i++] != 0);
	this->precipitate_only = (ints[i++] != 0);
	*ii = i;
	*dd = d;
}
#endif

void
cxxPPassemblageComp::totalize(PHREEQC_PTR_ARG)
{
	this->totals.clear();
	// component structures
	if (this->add_formula.size() != 0)
		return;
	struct phase *phase_ptr;
	int l;
	phase_ptr = P_INSTANCE_POINTER phase_bsearch(this->name.c_str(), &l, FALSE);
	if (phase_ptr != NULL)
	{
		cxxNameDouble phase_formula(phase_ptr->next_elt);
		this->totals.add_extensive(phase_formula, this->moles);
	}
	else
	{
		assert(false);
	}
	return;
}


void
cxxPPassemblageComp::add(const cxxPPassemblageComp & addee, LDBLE extensive)
{
	LDBLE ext1, ext2, f1, f2;
	if (extensive == 0.0)
		return;
	if (addee.name.size() == 0)
		return;
	// this and addee must have same name
	// otherwise generate a new PPassemblagComp with multiply

	ext1 = this->moles;
	ext2 = addee.moles * extensive;
	if (ext1 + ext2 != 0)
	{
		f1 = ext1 / (ext1 + ext2);
		f2 = ext2 / (ext1 + ext2);
	}
	else
	{
		f1 = 0.5;
		f2 = 0.5;
	}

	if (this->name.size() == 0 && addee.name.size() == 0)
	{
		return;
	}
	assert(this->name == addee.name);
	if (this->add_formula != addee.add_formula)
	{
		std::ostringstream oss;
		oss <<
			"Can not mix two Equilibrium_phases with differing add_formulae., "
			<< this->name;
		error_msg(oss.str().c_str(), CONTINUE);
		return;
	}
	this->si = this->si * f1 + addee.si * f2;
	this->si_org = this->si_org * f1 + addee.si_org * f2;
	this->moles += addee.moles * extensive;
	this->delta += addee.delta * extensive;
	this->initial_moles += addee.initial_moles * extensive;
}

void
cxxPPassemblageComp::multiply(LDBLE extensive)
{
	this->moles *= extensive;
	this->delta *= extensive;
	this->initial_moles *= extensive;
}
