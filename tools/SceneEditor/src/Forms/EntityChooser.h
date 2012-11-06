/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved. 
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(ENTITYCHOOSER_INCLUDED)
#define ENTITYCHOOSER_INCLUDED

class EntityChooser
	:	 public wxDialog
{
	wxListBox* m_listBox2;
	wxStdDialogButtonSizer* m_sdbSizer1;
	wxButton* m_sdbSizer1OK;
	wxButton* m_sdbSizer1Cancel;
	wxButton* m_cmdAdd;
	wxButton* m_cmdDelete;
	Teardrop::String m_name;

public:
	EntityChooser();
	~EntityChooser();

	const Teardrop::String& getEntityName() const;

	void OnSelectionChanged( wxCommandEvent& event );
	void OnSelectionDblClick( wxCommandEvent& event );
	void OnAdd( wxCommandEvent& event );
	void OnDelete( wxCommandEvent& event );
};

#endif // ENTITYCHOOSER_INCLUDED
