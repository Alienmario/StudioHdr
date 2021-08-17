//	StudioHdr is a SourcePawn library (.inc) that lets developers get information about entity models.
//	Copyright (C) 2021  Natanel 'LuqS' Shitrit

//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.

//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.

//	You should have received a copy of the GNU General Public License
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.

#if defined _studio_hdr_included
 #endinput
#endif
#define _studio_hdr_included

#define INVALID_ADDRESS_OFFSET view_as<Address>(-1)

// Offset for 'CStudioHdr *m_pStudioHdr' inside 'CBaseAnimating' class.
int offset_m_pStudioHdr;

// Offsets for 'studiohdr_t' struct members.
Address offset_numskinfamilies;
Address offset_numbodyparts;
Address offset_bodypartindex;
Address offset_numlocalattachments;
Address offset_localattachmentindex;
Address offset_numlocalposeparameters;
Address offset_localposeparamindex;

// Structs sizes
int sizeof_mstudiobodyparts_t;
int sizeof_mstudioattachment_t;
int sizeof_mstudioposeparamdesc_t;

// Offsets for 'mstudiobodyparts_t' struct members.
Address offset_nummodels;
Address offset_base;

// Offsets for 'mstudioposeparamdesc_t' struct members.
Address offset_start;
Address offset_end;

// General offsets.
Address offset_sznameindex;

// Object that represents an address.
methodmap AddressObject
{
	// Make sure the this address is not 0.
	public void Validate()
	{
		if (view_as<Address>(this) == Address_Null)
		{
			ThrowError("Invalid Object.");
		}
	}
}

// Base functions for StudioHdr objects.
methodmap BaseStudioHdrMember < AddressObject
{
	// Get the index to the name pointer.
	property Address sznameindex
	{
		public get()
		{
			this.Validate();
			
			// 'this' [base] + 'sznameindex' [offset]
			return LoadFromAddress(view_as<Address>(this) + offset_sznameindex, NumberType_Int32);
		}
	}
	
	// Get StudioHdr object name.
	public void GetName(char[] name, int name_len)
	{
		this.Validate();
		
		// Load the string from the address of the name.
		LoadStringFromAddress(view_as<Address>(this) + this.sznameindex, name, name_len);
	}
}

// BodyPart //
enum BodyPart
{
	NULL_BODY_PART
}

methodmap BodyPart < BaseStudioHdrMember
{
	public BodyPart(Address studio_hdr_base, int part_index)
	{
		// 'studiohdr_t' [base] + 'bodypartindex' [offset] + (size of 'mstudiobodyparts_t' * index [get item based on index])
		return view_as<BodyPart>(studio_hdr_base + LoadFromAddress(studio_hdr_base + offset_bodypartindex, NumberType_Int32) + view_as<Address>(sizeof_mstudiobodyparts_t * part_index));
	}
	
	property int nummodels
	{
		public get()
		{
			this.Validate();
			
			// 'this' [base] + 'nummodels' [offset]
			return LoadFromAddress(view_as<Address>(this) + offset_nummodels, NumberType_Int32);
		}
	}
	
	property int base
	{
		public get()
		{
			this.Validate();
			
			// 'this' [base] + 'base' [offset]
			return LoadFromAddress(view_as<Address>(this) + offset_base, NumberType_Int32);
		}
	}
}

// Attachment //
enum Attachment
{
	NULL_ATTACHMENT
}

methodmap Attachment < BaseStudioHdrMember
{
	public Attachment(Address studio_hdr_base, int attachment_index)
	{
		// 'studiohdr_t' [base] + 'localattachmentindex' [offset] + (size of 'mstudioattachment_t' * index [get item based on index])
		return view_as<Attachment>(studio_hdr_base + LoadFromAddress(studio_hdr_base + offset_localattachmentindex, NumberType_Int32) + view_as<Address>(sizeof_mstudioattachment_t * attachment_index));
	}
}

// PoseParameter //
enum PoseParameter
{
	NULL_POSE_PARAMETER
}

methodmap PoseParameter < BaseStudioHdrMember
{
	public PoseParameter(Address studio_hdr_base, int pose_parameter_index)
	{
		// 'studiohdr_t' [base] + 'localposeparamindex' [offset] + (size of 'mstudioposeparamdesc_t' * index [get item based on index])
		return view_as<PoseParameter>(studio_hdr_base + LoadFromAddress(studio_hdr_base + offset_localposeparamindex, NumberType_Int32) + view_as<Address>(sizeof_mstudioposeparamdesc_t * pose_parameter_index));
	}
	
	property float start
	{
		public get()
		{
			this.Validate();
			
			// 'this' [base] + 'start' [offset]
			return LoadFromAddress(view_as<Address>(this) + offset_start, NumberType_Int32);
		}
	}
	
	property float end
	{
		public get()
		{
			this.Validate();
			
			// 'this' [base] + 'end' [offset]
			return LoadFromAddress(view_as<Address>(this) + offset_end, NumberType_Int32);
		}
	}
}

// StudioHdr //
enum StudioHdr
{
	NULL_STUDIO_HDR
}

methodmap StudioHdr < AddressObject
{
	// Constructor [entity]
	public StudioHdr(int entity)
	{
		// If the entity doesn't have 'm_hLightingOrigin' prop, it's not a CBaseAnimating.
		if (!HasEntProp(entity, Prop_Data, "m_hLightingOrigin"))
		{
			// "Invalid" entity sent.
			return NULL_STUDIO_HDR;
		}
		
		// Load 'studiohdr_t m_pStudioHdr' from 'CStudioHdr m_pStudioHdr' pointer. (can be treated as if it was a studiohdr_t **)
		return view_as<StudioHdr>(LoadFromAddress(view_as<Address>(GetEntData(entity, offset_m_pStudioHdr)), NumberType_Int32));
	}
	
	// Skin Families
	property int SkinFamiliesCount
	{
		public get()
		{
			this.Validate();
			
			// 'this' [base] + 'numskinfamilies' [offset]
			return LoadFromAddress(view_as<Address>(this) + offset_numskinfamilies, NumberType_Int32);
		}
	}
	
	// Body groups
	property int NumBodyParts
	{
		public get()
		{
			this.Validate();
			
			// 'this' [base] + 'numbodyparts' [offset]
			return LoadFromAddress(view_as<Address>(this) + offset_numbodyparts, NumberType_Int32);
		}
	}
	
	public BodyPart GetBodyPart(int part_index)
	{
		this.Validate();
		
		// Make sure the part index is in valid range.
		return (0 <= part_index < this.NumBodyParts) ? BodyPart(view_as<Address>(this), part_index) : NULL_BODY_PART;
	}
	
	// Finds a body part index by name.
	public int FindBodyPart(char[] name)
	{
		this.Validate();
		
		char current_name[64];
		for (int current_body_part, num_body_parts = this.NumBodyParts; current_body_part < num_body_parts; current_body_part++)
        {
        	// Get current body part.
            this.GetBodyPart(current_body_part).GetName(current_name, sizeof(current_name));
            
            // Compare current name to the given name.
            if (StrEqual(current_name, name))
            {
            	// Return the index if it was the body part searched for.
            	return current_body_part;
            }
        }
        
        // If it wasn't found, this will be invalid index.
		return -1;
	}
	
	public bool CalculateNewBodyGroup(int &body, int part_index, int new_value)
	{
		this.Validate();
		
		// Get body part.
		BodyPart body_part = this.GetBodyPart(part_index);
		
		// Get body part members.
		int body_nummodels = body_part.nummodels, body_base = body_part.base;
		
		// Check that nummodels is valid.
		if (body_nummodels == -1 || new_value >= body_nummodels)
		{
			// failed.
			return false;
		}
		
		// calculate new bodygroup.
		body += (new_value - body / body_base % body_nummodels) * body_base;
		
		// success.
		return true;
	}
	
	// Attachements
	property int AttachmentCount
	{
		public get()
		{
			this.Validate();
			
			// 'this' [base] + 'numlocalattachments' [offset]
			return LoadFromAddress(view_as<Address>(this) + offset_numlocalattachments, NumberType_Int32);
		}
	}
	
	public Attachment GetAttachment(int attachment_index)
	{
		this.Validate();
			
		// Make sure the attachment index is in valid range.
		return (0 <= attachment_index < this.AttachmentCount) ? Attachment(view_as<Address>(this), attachment_index) : NULL_ATTACHMENT;
	}
	
	// Finds a attachment index by name.
	public int FindAttachment(char[] name)
	{
		this.Validate();
		
		char current_name[64];
		for (int current_attachment, attachment_count = this.AttachmentCount; current_attachment < attachment_count; current_attachment++)
        {
        	// Get current attachment.
            this.GetAttachment(current_attachment).GetName(current_name, sizeof(current_name));
            
            // Compare current name to the given name.
            if (StrEqual(current_name, name))
            {
            	// Return the index if it was the attachment searched for.
            	return current_attachment;
            }
        }
        
        // If it wasn't found, this will be invalid index.
		return -1;
	}
	
	// Pose Parameters
	property int NumPoseParameters
	{
		public get()
		{
			this.Validate();
			
			// 'this' [base] + 'numlocalposeparameters' [offset]
			return LoadFromAddress(view_as<Address>(this) + offset_numlocalposeparameters, NumberType_Int32);
		}
	}
	
	public PoseParameter GetPoseParameter(int pose_parameter_index)
	{
		this.Validate();
			
		// Make sure the pose parameter index is in valid range.
		return (0 <= pose_parameter_index < this.NumPoseParameters) ? PoseParameter(view_as<Address>(this), pose_parameter_index) : NULL_POSE_PARAMETER;
	}
	
	// Finds a pose parameter index by name.
	public int FindPoseParameter(char[] name)
	{
		this.Validate();
		
		char current_name[64];
		for (int current_pose_parameter, num_pose_parameters = this.AttachmentCount; current_pose_parameter < num_pose_parameters; current_pose_parameter++)
        {
        	// Get current pose parameter.
            this.GetPoseParameter(current_pose_parameter).GetName(current_name, sizeof(current_name));
            
            // Compare current name to the given name.
            if (StrEqual(current_name, name))
            {
            	// Return the index if it was the pose parameter searched for.
            	return current_pose_parameter;
            }
        }
        
        // If it wasn't found, this will be invalid index.
		return -1;
	}
}

stock void LoadStringFromAddress(Address charptr, char[] buffer, int buffer_size)
{
	int current_char_index;
	while (current_char_index < buffer_size && (buffer[current_char_index] = view_as<char>(LoadFromAddress(charptr + view_as<Address>(current_char_index), NumberType_Int8))))
	{
		current_char_index++;
	}
	
	buffer[current_char_index] = '\0';
}

public void OnPluginStart()
{
	GameData gamedata = new GameData("studio_hdr.games");
	
	if (!gamedata)
	{
		SetFailState("Couldn't find gamedata file 'studio_hdr.games.txt'.");
	}
	
	// CBaseAnimating::m_pStudioHdr
	if ((offset_m_pStudioHdr = FindSendPropInfo("CBaseAnimating", "m_hLightingOrigin")) <= 0)
	{
		SetFailState("Failed to get 'CBaseAnimating::m_hLightingOrigin' offset.");
	}
	
	int offset = gamedata.GetOffset("CBaseAnimating::StudioHdr");
	if (offset == -1)
	{
		SetFailState("Failed to get 'CBaseAnimating::StudioHdr' offset.");
	}
	
	offset_m_pStudioHdr += offset;
	
	// [offset] studiohdr_t::numskinfamilies
	if ((offset_numskinfamilies = view_as<Address>(gamedata.GetOffset("studiohdr_t::numskinfamilies"))) == INVALID_ADDRESS_OFFSET)
	{
		SetFailState("Failed to get 'studiohdr_t::numskinfamilies' offset.");
	}
	
	// [offset] studiohdr_t::numbodyparts
	if ((offset_numbodyparts = view_as<Address>(gamedata.GetOffset("studiohdr_t::numbodyparts"))) == INVALID_ADDRESS_OFFSET)
	{
		SetFailState("Failed to get 'studiohdr_t::numbodyparts' offset.");
	}
	
	// [offset] studiohdr_t::bodypartindex
	if ((offset_bodypartindex = view_as<Address>(gamedata.GetOffset("studiohdr_t::bodypartindex"))) == INVALID_ADDRESS_OFFSET)
	{
		SetFailState("Failed to get 'studiohdr_t::bodypartindex' offset.");
	}
	
	// [offset] studiohdr_t::numlocalattachments
	if ((offset_numlocalattachments = view_as<Address>(gamedata.GetOffset("studiohdr_t::numlocalattachments"))) == INVALID_ADDRESS_OFFSET)
	{
		SetFailState("Failed to get 'studiohdr_t::numlocalattachments' offset.");
	}
	
	// [offset] studiohdr_t::localattachmentindex
	if ((offset_localattachmentindex = view_as<Address>(gamedata.GetOffset("studiohdr_t::localattachmentindex"))) == INVALID_ADDRESS_OFFSET)
	{
		SetFailState("Failed to get 'studiohdr_t::localattachmentindex' offset.");
	}
	
	// [offset] studiohdr_t::numlocalposeparameters
	if ((offset_numlocalposeparameters = view_as<Address>(gamedata.GetOffset("studiohdr_t::numlocalposeparameters"))) == INVALID_ADDRESS_OFFSET)
	{
		SetFailState("Failed to get 'studiohdr_t::numlocalposeparameters' offset.");
	}
	
	// [offset] studiohdr_t::localposeparamindex
	if ((offset_localposeparamindex = view_as<Address>(gamedata.GetOffset("studiohdr_t::localposeparamindex"))) == INVALID_ADDRESS_OFFSET)
	{
		SetFailState("Failed to get 'studiohdr_t::localposeparamindex' offset.");
	}
	
	// [offset] mstudiobodyparts_t::nummodels
	if ((offset_nummodels = view_as<Address>(gamedata.GetOffset("mstudiobodyparts_t::nummodels"))) == INVALID_ADDRESS_OFFSET)
	{
		SetFailState("Failed to get 'mstudiobodyparts_t::nummodels' offset.");
	}
	
	// [offset] mstudioposeparamdesc_t::start
	if ((offset_start = view_as<Address>(gamedata.GetOffset("mstudioposeparamdesc_t::start"))) == INVALID_ADDRESS_OFFSET)
	{
		SetFailState("Failed to get 'mstudioposeparamdesc_t::start' offset.");
	}
	
	// [offset] mstudioposeparamdesc_t::end
	if ((offset_end = view_as<Address>(gamedata.GetOffset("mstudioposeparamdesc_t::end"))) == INVALID_ADDRESS_OFFSET)
	{
		SetFailState("Failed to get 'mstudioposeparamdesc_t::end' offset.");
	}
	
	// [offset] mstudiobodyparts_t::base
	if ((offset_base = view_as<Address>(gamedata.GetOffset("mstudiobodyparts_t::base"))) == INVALID_ADDRESS_OFFSET)
	{
		SetFailState("Failed to get 'mstudiobodyparts_t::base' offset.");
	}
	
	// [offset] sznameindex
	if ((offset_sznameindex = view_as<Address>(gamedata.GetOffset("sznameindex"))) == INVALID_ADDRESS_OFFSET)
	{
		SetFailState("Failed to get 'sznameindex' offset.");
	}
	
	// [sizeof] mstudiobodyparts_t
	if ((sizeof_mstudiobodyparts_t = gamedata.GetOffset("mstudiobodyparts_t::sizeof")) == -1)
	{
		SetFailState("Failed to get 'mstudiobodyparts_t::sizeof' offset.");
	}
	
	// [sizeof] mstudioattachment_t
	if ((sizeof_mstudioattachment_t = gamedata.GetOffset("mstudioattachment_t::sizeof")) == -1)
	{
		SetFailState("Failed to get 'mstudioattachment_t::sizeof' offset.");
	}
	
	// [sizeof] mstudioposeparamdesc_t
	if ((sizeof_mstudioposeparamdesc_t = gamedata.GetOffset("mstudioposeparamdesc_t::sizeof")) == -1)
	{
		SetFailState("Failed to get 'mstudioposeparamdesc_t::sizeof' offset.");
	}
	
	delete gamedata;
	
	_OnPluginStart();
}

#define OnPluginStart _OnPluginStart