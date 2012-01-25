// Surface.cxx: implementation of the cxxSurface class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include <cassert>				// assert
#include <algorithm>			// std::sort
#include "Phreeqc_class.h"
#include "Utils.h"				// define first
#include "Phreeqc.h"
#include "Surface.h"
#include "cxxMix.h"
#include "phqalloc.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxSurface::cxxSurface(PHRQ_io *io)
	//
	// default constructor for cxxSurface 
	//
:	cxxNumKeyword(io)
{
	type = DDL;
	dl_type = NO_DL;
	sites_units = SITES_ABSOLUTE;
	//diffuse_layer           = false;
	//edl                     = false;
	only_counter_ions = false;
	//donnan                  = false;
	thickness = 1e-8;
	debye_lengths = 0.0;
	DDL_viscosity = 1.0;
	DDL_limit = 0.8;
	transport = false;
}

cxxSurface::cxxSurface(struct surface *surface_ptr, PHRQ_io *io)
		//
		// constructor for cxxSurface from struct surface
		//
	:
cxxNumKeyword(io)
{
	int i;

	this->Set_description(surface_ptr->description);
	n_user = surface_ptr->n_user;
	n_user_end = surface_ptr->n_user_end;
	type = surface_ptr->type;
	dl_type = surface_ptr->dl_type;
	sites_units = surface_ptr->sites_units;
	//diffuse_layer                = (surface_ptr->diffuse_layer == TRUE); 
	//edl                          = (surface_ptr->edl == TRUE); 
	only_counter_ions = (surface_ptr->only_counter_ions == TRUE);
	//donnan                       = (surface_ptr->donnan == TRUE); 
	thickness = surface_ptr->thickness;
	debye_lengths = surface_ptr->debye_lengths;
	DDL_viscosity = surface_ptr->DDL_viscosity;
	DDL_limit = surface_ptr->DDL_limit;
	transport = (surface_ptr->transport == TRUE);
	// Surface components
	for (i = 0; i < surface_ptr->count_comps; i++)
	{
		cxxSurfaceComp ec(&(surface_ptr->comps[i]), this->Get_io());
		std::string str(ec.Get_formula());
		surfaceComps[str] = ec;
	}
	// Surface charge
	for (i = 0; i < surface_ptr->count_charge; i++)
	{
		cxxSurfaceCharge ec(&(surface_ptr->charge[i]), this->Get_io());
		std::string str(ec.Get_name());
		surfaceCharges[str] = ec;
	}
}

cxxSurface::cxxSurface(const std::map < int, cxxSurface > &entities,
					   cxxMix & mix, int l_n_user, PHRQ_io *io):
cxxNumKeyword(io)
{
	this->n_user = this->n_user_end = l_n_user;
	type = DDL;
	dl_type = NO_DL;
	sites_units = SITES_ABSOLUTE;
	only_counter_ions = false;
	thickness = 1e-8;
	debye_lengths = 0.0;
	DDL_viscosity = 1.0;
	DDL_limit = 0.8;
	transport = false;
//
//   Mix exchangers
//
	const std::map < int, LDBLE >&mixcomps = mix.Get_mixComps();
	std::map < int, LDBLE >::const_iterator it;
	for (it = mixcomps.begin(); it != mixcomps.end(); it++)
	{
		if (entities.find(it->first) != entities.end())
		{
			const cxxSurface *entity_ptr =
				&(entities.find(it->first)->second);
			this->add(*entity_ptr, it->second);
		}
	}
}

cxxSurface::~cxxSurface()
{
}

bool
cxxSurface::Get_related_phases() const
{
	for (std::map < std::string, cxxSurfaceComp >::const_iterator it =
		 this->surfaceComps.begin(); it != this->surfaceComps.end(); ++it)
	{
		if ((*it).second.Get_phase_name().size() == 0)
			continue;
		return (true);
	}
	return (false);
}

bool
cxxSurface::Get_related_rate() const
{
	for (std::map < std::string, cxxSurfaceComp >::const_iterator it =
		 this->surfaceComps.begin(); it != this->surfaceComps.end(); ++it)
	{
		if ((*it).second.Get_rate_name().size() == 0)
			continue;
		return (true);
	}
	return (false);
}

void
cxxSurface::dump_xml(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing surface message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// Surface element and attributes
	s_oss << indent0;
	s_oss << "<surface " << "\n";

	s_oss << indent1;
	//s_oss << "diffuse_layer=\"" << this->diffuse_layer << "\"" << "\n";
	s_oss << "surface_type=\"" << this->type << "\"" << "\n";

	s_oss << indent1;
	//s_oss << "edl=\"" << this->edl << "\"" << "\n";
	s_oss << "dl_type=\"" << this->dl_type << "\"" << "\n";

	s_oss << indent1;
	s_oss << "sites_units=\"" << this->sites_units << "\"" << "\n";

	s_oss << indent1;
	s_oss << "only_counter_ions=\"" << this->
		only_counter_ions << "\"" << "\n";

	//s_oss << indent1;
	//s_oss << "donnan=\"" << this->donnan << "\"" << "\n";

	s_oss << indent1;
	s_oss << "thickness=\"" << this->thickness << "\"" << "\n";

	s_oss << indent1;
	s_oss << "debye_lengths=\"" << this->debye_lengths << "\"" << "\n";

	s_oss << indent1;
	s_oss << "DDL_viscosity=\"" << this->DDL_viscosity << "\"" << "\n";

	s_oss << indent1;
	s_oss << "DDL_limit=\"" << this->DDL_limit << "\"" << "\n";

	s_oss << indent1;
	s_oss << "transport=\"" << this->transport << "\"" << "\n";

	// surface component structures
	s_oss << indent1;
	s_oss << "<component " << "\n";
	{
		for (std::map < std::string, cxxSurfaceComp >::const_iterator it =
			 this->surfaceComps.begin(); it != this->surfaceComps.end(); ++it)
		{
			(*it).second.dump_xml(s_oss, indent + 2);
		}
	}
	// surface charge structures
	s_oss << indent1;
	s_oss << "<charge_component " << "\n";
	for (std::map < std::string, cxxSurfaceCharge >::const_iterator it =
		 surfaceCharges.begin(); it != surfaceCharges.end(); ++it)
	{
		(*it).second.dump_xml(s_oss, indent + 2);
	}

	return;
}

void
cxxSurface::dump_raw(std::ostream & s_oss, unsigned int indent, int *n_out) const
{
	//const char    ERR_MESSAGE[] = "Packing surface message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// Surface element and attributes
	s_oss << indent0;
	int n_user_local = (n_out != NULL) ? *n_out : this->n_user;
	s_oss << "SURFACE_RAW       " << n_user_local << " " << this->description << "\n";

	s_oss << indent1;
	//s_oss << "-diffuse_layer " << this->diffuse_layer << "\n";
	s_oss << "-type " << this->type << "\n";

	s_oss << indent1;
	//s_oss << "-edl " << this->edl << "\n";
	s_oss << "-dl_type " << this->dl_type << "\n";

	s_oss << indent1;
	s_oss << "-sites_units " << this->sites_units << "\n";

	s_oss << indent1;
	s_oss << "-only_counter_ions " << this->only_counter_ions << "\n";

	//s_oss << indent1;
	//s_oss << "-donnan " << this->donnan << "\n";

	s_oss << indent1;
	s_oss << "-thickness " << this->thickness << "\n";

	s_oss << indent1;
	s_oss << "-debye_lengths " << this->debye_lengths << "\n";

	s_oss << indent1;
	s_oss << "-DDL_viscosity " << this->DDL_viscosity << "\n";

	s_oss << indent1;
	s_oss << "-DDL_limit " << this->DDL_limit << "\n";

	s_oss << indent1;
	s_oss << "-transport " << this->transport << "\n";

	// surfaceComps structures
	for (std::map < std::string, cxxSurfaceComp >::const_iterator it =
		 surfaceComps.begin(); it != surfaceComps.end(); ++it)
	{
		s_oss << indent1;
		s_oss << "-component" << "\n";
		(*it).second.dump_raw(s_oss, indent + 2);
	}
	// surface charge structures
	{
		for (std::map < std::string, cxxSurfaceCharge >::const_iterator it =
			 surfaceCharges.begin(); it != surfaceCharges.end(); ++it)
		{
			s_oss << indent1;
			s_oss << "-charge_component " << "\n";
			(*it).second.dump_raw(s_oss, indent + 2);
		}
	}

	return;
}

void
cxxSurface::read_raw(CParser & parser, bool check)
{
	static std::vector < std::string > vopts;
	int i = 0;
	if (vopts.empty())
	{
		vopts.reserve(15);
		vopts.push_back("diffuse_layer");	// 0 
		vopts.push_back("edl");	// 1 
		vopts.push_back("only_counter_ions");	// 2 
		vopts.push_back("donnan");	// 3 
		vopts.push_back("thickness");	// 4 
		vopts.push_back("component");	// 5
		vopts.push_back("charge_component");	// 6 
		vopts.push_back("type ");	// 7
		vopts.push_back("dl_type");	// 8 
		vopts.push_back("sites_units");	// 9 
		vopts.push_back("debye_lengths");	// 10
		vopts.push_back("ddl_viscosity");	// 11
		vopts.push_back("ddl_limit");	// 12
		vopts.push_back("transport");	// 13
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;
	bool useLastLine(false);

	// Read surface number and description
	this->read_number_description(parser);

	opt_save = CParser::OPT_ERROR;
	//bool diffuse_layer_defined(false); 
	//bool edl_defined(false); 
	bool only_counter_ions_defined(false);
	//bool donnan_defined(false); 
	bool thickness_defined(false);
	bool type_defined(false);
	bool dl_type_defined(false);
	bool sites_units_defined(false);
	bool debye_lengths_defined(false);
	bool DDL_viscosity_defined(false);
	bool DDL_limit_defined(false);
	bool transport_defined(false);

	for (;;)
	{
		int opt;
		if (useLastLine == false)
		{
			opt = parser.get_option(vopts, next_char);
		}
		else
		{
			opt = parser.getOptionFromLastLine(vopts, next_char, true);
		}
		switch (opt)
		{
		case CParser::OPT_EOF:
			break;
		case CParser::OPT_KEYWORD:
			break;
		case CParser::OPT_DEFAULT:
		case CParser::OPT_ERROR:
			opt = CParser::OPT_EOF;
			parser.error_msg("Unknown input in SURFACE keyword.",
							 PHRQ_io::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), PHRQ_io::OT_CONTINUE);
			useLastLine = false;
			break;

		case 0:				// diffuse_layer
			parser.incr_input_error();
			parser.error_msg("Diffuse layer is obsolete, use -type.",
							 PHRQ_io::OT_CONTINUE);
			/*
			   if (!(parser.get_iss() >> this->diffuse_layer))
			   {
			   this->diffuse_layer = false;
			   parser.incr_input_error();
			   parser.error_msg("Expected boolean value for diffuse_layer.", PHRQ_io::OT_CONTINUE);
			   }
			   diffuse_layer_defined = true;
			 */
			useLastLine = false;
			break;

		case 1:				// edl
			parser.incr_input_error();
			parser.error_msg("-edl is obsolete, use -type.",
							 PHRQ_io::OT_CONTINUE);
			/*
			   if (!(parser.get_iss() >> this->edl))
			   {
			   this->edl = false;
			   parser.incr_input_error();
			   parser.error_msg("Expected boolean value for edl.", PHRQ_io::OT_CONTINUE);
			   }
			   edl_defined = true;
			 */
			useLastLine = false;
			break;

		case 2:				// only_counter_ions
			if (!(parser.get_iss() >> this->only_counter_ions))
			{
				this->only_counter_ions = false;
				parser.incr_input_error();
				parser.
					error_msg("Expected boolean value for only_counter_ions.",
							  PHRQ_io::OT_CONTINUE);
			}
			only_counter_ions_defined = true;
			useLastLine = false;
			break;

		case 3:				// donnan
			parser.incr_input_error();
			parser.error_msg("-Donnan is obsolete, use -dl_type.",
							 PHRQ_io::OT_CONTINUE);
			/*
			   if (!(parser.get_iss() >> this->donnan))
			   {
			   this->donnan = false;
			   parser.incr_input_error();
			   parser.error_msg("Expected boolean value for donnan.", PHRQ_io::OT_CONTINUE);
			   }
			   donnan_defined = true;
			 */
			useLastLine = false;
			break;

		case 4:				// thickness
			if (!(parser.get_iss() >> this->thickness))
			{
				this->thickness = 0.0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for thickness.",
								 PHRQ_io::OT_CONTINUE);
			}
			thickness_defined = true;
			useLastLine = false;
			break;

		case 5:				// component
			{
				cxxSurfaceComp ec(this->Get_io());

				// preliminary read
				parser.set_accumulate(true);
				ec.read_raw(parser, false);
				parser.set_accumulate(false);
				std::istringstream is(parser.get_accumulated());
				CParser reread(is, this->Get_io());
				reread.set_echo_file(CParser::EO_NONE);
				reread.set_echo_stream(CParser::EO_NONE);
				if (this->surfaceComps.find(ec.Get_formula()) != this->surfaceComps.end())
				{
					cxxSurfaceComp & comp = this->surfaceComps.find(ec.Get_formula())->second;
					comp.read_raw(reread, false);
				}
				else
				{
					cxxSurfaceComp ec1(this->Get_io());
					ec1.read_raw(reread, false);
					std::string str(ec1.Get_formula());
					this->surfaceComps[str] = ec1;
				}
			}
			useLastLine = true;
			break;

		case 6:				// charge_component
			{
				cxxSurfaceCharge ec(this->Get_io());

				// preliminary read
				parser.set_accumulate(true);
				ec.read_raw(parser, false);
				parser.set_accumulate(false);
				std::istringstream is(parser.get_accumulated());
				CParser reread(is, this->Get_io());
				reread.set_echo_file(CParser::EO_NONE);
				reread.set_echo_stream(CParser::EO_NONE);
				if (this->surfaceCharges.find(ec.Get_name()) != this->surfaceCharges.end())
				{
					cxxSurfaceCharge & comp = this->surfaceCharges.find(ec.Get_name())->second;
					comp.read_raw(reread, false);
				}
				else
				{
					cxxSurfaceCharge ec1(this->Get_io());
					ec1.read_raw(reread, false);
					std::string str(ec1.Get_name());
					this->surfaceCharges[str] = ec1;
				}
			}
			useLastLine = true;
			break;
		case 7:				// type
			i = 0;
			if (!(parser.get_iss() >> i))
			{
				this->type = NO_EDL;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for type.",
								 PHRQ_io::OT_CONTINUE);
			}
			this->type = (SURFACE_TYPE) i;
			type_defined = true;
			useLastLine = false;
			break;
		case 8:				// dl_type
			i = 0;
			if (!(parser.get_iss() >> i))
			{
				this->dl_type = NO_DL;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for dl_type.",
								 PHRQ_io::OT_CONTINUE);
			}
			this->dl_type = (DIFFUSE_LAYER_TYPE) i;
			dl_type_defined = true;
			useLastLine = false;
			break;
		case 9:				// sites_units
			i = 0;
			if (!(parser.get_iss() >> i))
			{
				this->sites_units = SITES_ABSOLUTE;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for sites_units.",
								 PHRQ_io::OT_CONTINUE);
			}
			this->sites_units = (SITES_UNITS) i;
			sites_units_defined = true;
			useLastLine = false;
			break;

		case 10:				// debye_lengths
			if (!(parser.get_iss() >> this->debye_lengths))
			{
				this->debye_lengths = 0.0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for debye_lengths.",
								 PHRQ_io::OT_CONTINUE);
			}
			debye_lengths_defined = true;
			useLastLine = false;
			break;

		case 11:				// DDL_viscosity
			if (!(parser.get_iss() >> this->DDL_viscosity))
			{
				this->DDL_viscosity = 0.0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for DDL_viscosity.",
								 PHRQ_io::OT_CONTINUE);
			}
			DDL_viscosity_defined = true;
			useLastLine = false;
			break;

		case 12:				// DDL_limit
			if (!(parser.get_iss() >> this->DDL_limit))
			{
				this->DDL_limit = 0.0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for DDL_limit.",
								 PHRQ_io::OT_CONTINUE);
			}
			DDL_limit_defined = true;
			useLastLine = false;
			break;

		case 13:				// transport
			if (!(parser.get_iss() >> this->transport))
			{
				this->transport = false;
				parser.incr_input_error();
				parser.error_msg("Expected boolean value for transport.",
								 PHRQ_io::OT_CONTINUE);
			}
			transport_defined = true;
			useLastLine = false;
			break;
		}

		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	if (check)
	{
		// members that must be defined
		/*
		if (diffuse_layer_defined == false) {
		parser.incr_input_error();
		parser.error_msg("Diffuse_layer not defined for SURFACE_RAW input.", PHRQ_io::OT_CONTINUE);
		}
		if (edl_defined == false) {
		parser.incr_input_error();
		parser.error_msg("Edl not defined for SURFACE_RAW input.", PHRQ_io::OT_CONTINUE);
		}
		*/
		if (only_counter_ions_defined == false)
		{
			parser.incr_input_error();
			parser.
				error_msg("Only_counter_ions not defined for SURFACE_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		/*
		if (donnan_defined == false) {
		parser.incr_input_error();
		parser.error_msg("Donnan not defined for SURFACE_RAW input.", PHRQ_io::OT_CONTINUE);
		}
		*/
		if (thickness_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Thickness not defined for SURFACE_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (type_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Surface type not defined for SURFACE_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (dl_type_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Dl_type not defined for SURFACE_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (sites_units_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Sites_units not defined for SURFACE_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (debye_lengths_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Debye_lengths not defined for SURFACE_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (DDL_viscosity_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("DDL_viscosity not defined for SURFACE_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (DDL_limit_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("DDL_limit not defined for SURFACE_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (transport_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Transport not defined for SURFACE_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
	}
}

#ifdef USE_MPI
/* ---------------------------------------------------------------------- */
void
cxxSurface::mpi_pack(std::vector < int >&ints, std::vector < LDBLE >&doubles)
/* ---------------------------------------------------------------------- */
{
	/* int n_user; */
	ints.push_back(this->n_user);
	ints.push_back((int) this->surfaceComps.size());
	for (std::map < std::string, cxxSurfaceComp >::iterator it =
		 this->surfaceComps.begin(); it != this->surfaceComps.end(); it++)
	{
		(*it).second.mpi_pack(ints, doubles);
	}
	ints.push_back((int) this->surfaceCharges.size());
	for (std::map < std::string, cxxSurfaceCharge >::iterator it =
		 this->surfaceCharges.begin(); it != this->surfaceCharges.end(); it++)
	{
		(*it).second.mpi_pack(ints, doubles);
	}
	//ints.push_back((int) this->diffuse_layer);
	//ints.push_back((int) this->edl);
	ints.push_back((int) this->type);
	ints.push_back((int) this->dl_type);
	ints.push_back((int) this->sites_units);

	ints.push_back((int) this->only_counter_ions);
	//ints.push_back((int) this->donnan);
	doubles.push_back(this->thickness);
	doubles.push_back(this->debye_lengths);
	doubles.push_back(this->DDL_viscosity);
	doubles.push_back(this->DDL_limit);
	ints.push_back((int) this->transport);
}

/* ---------------------------------------------------------------------- */
void
cxxSurface::mpi_unpack(int *ints, int *ii, LDBLE *doubles, int *dd)
/* ---------------------------------------------------------------------- */
{
	int i = *ii;
	int d = *dd;
	/* int n_user; */
	this->n_user = ints[i++];
	this->n_user_end = this->n_user;
	this->description = " ";

	int count = ints[i++];
	this->surfaceComps.clear();
	for (int n = 0; n < count; n++)
	{
		cxxSurfaceComp sc;
		sc.mpi_unpack(ints, &i, doubles, &d);
		std::string str(sc.get_formula());
		this->surfaceComps[str] = sc;
	}
	count = ints[i++];
	this->surfaceCharges.clear();
	for (int n = 0; n < count; n++)
	{
		cxxSurfaceCharge sc;
		sc.mpi_unpack(ints, &i, doubles, &d);
		std::string str(sc.get_name());
		this->surfaceCharges[str] = sc;
	}
	//this->diffuse_layer = (bool) ints[i++];
	//this->edl = (bool) ints[i++];
	this->type = (SURFACE_TYPE) ints[i++];
	this->dl_type = (DIFFUSE_LAYER_TYPE) ints[i++];
	this->sites_units = (SITES_UNITS) ints[i++];
	this->only_counter_ions = (ints[i++] == TRUE);
	//this->donnan = (bool) ints[i++];
	this->thickness = doubles[d++];
	this->debye_lengths = doubles[d++];
	this->DDL_viscosity = doubles[d++];
	this->DDL_limit = doubles[d++];
	this->transport = (ints[i++] == TRUE);
	*ii = i;
	*dd = d;
}
#endif

void
cxxSurface::totalize()
{
	this->totals.clear();
	// component structures
	for (std::map < std::string, cxxSurfaceComp >::const_iterator it =
		 surfaceComps.begin(); it != surfaceComps.end(); ++it)
	{
		this->totals.add_extensive((*it).second.Get_totals(), 1.0);
		this->totals.add("Charge", (*it).second.Get_charge_balance());
	}
	return;
}

void
cxxSurface::add(const cxxSurface & addee, LDBLE extensive)
		//
		// Add surface to "this" exchange
		//
{
	if (extensive == 0.0)
		return;
	if (this->surfaceComps.size() == 0)
	{
		//enum SURFACE_TYPE type;
		this->type = addee.type;
		//enum DIFFUSE_LAYER_TYPE dl_type;
		this->dl_type = addee.dl_type;
		//enum SITES_UNITS sites_units;
		this->sites_units = addee.sites_units;
		//bool only_counter_ions;
		this->only_counter_ions = addee.only_counter_ions;
		//LDBLE thickness;
		this->thickness = addee.thickness;
		//LDBLE debye_lengths;
		this->debye_lengths = addee.debye_lengths;
		//LDBLE DDL_viscosity;
		this->DDL_viscosity = addee.DDL_viscosity;
		//LDBLE DDL_limit;
		this->DDL_limit = addee.DDL_limit;
		//bool transport;
		this->transport = addee.transport;
	}

	//std::map <std::string, cxxSurfaceComp> surfaceComps;
	for (std::map < std::string, cxxSurfaceComp >::const_iterator itadd =
		 addee.surfaceComps.begin(); itadd != addee.surfaceComps.end();
		 ++itadd)
	{
		std::map < std::string, cxxSurfaceComp >::iterator it = this->surfaceComps.find((*itadd).first);
		if (it != this->surfaceComps.end())
		{
			(*it).second.add((*itadd).second, extensive);
		}
		else
		{
			cxxSurfaceComp entity = (*itadd).second;
			entity.multiply(extensive);
			std::string str(entity.Get_formula());
			this->surfaceComps[str] = entity;
		}
	}

	//std::map < std::string, cxxSurfaceCharge > surfaceCharges;
	for (std::map < std::string, cxxSurfaceCharge >::const_iterator itadd =
		 addee.surfaceCharges.begin(); itadd != addee.surfaceCharges.end();
		 ++itadd)
	{
		std::map < std::string, cxxSurfaceCharge >::iterator it = this->surfaceCharges.find((*itadd).first);
		if (it != this->surfaceCharges.end())
		{
			(*it).second.add( (*itadd).second, extensive);
		}
		else
		{
			cxxSurfaceCharge entity = (*itadd).second;
			entity.multiply(extensive);
			std::string str(entity.Get_name());
			this->surfaceCharges[str] = entity;
		}
	}
}
