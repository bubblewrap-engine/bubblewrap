#include "Bubblewrap/Managers/Managers.hpp"
#include "Bubblewrap/Render/Vertices.hpp"
#include "Bubblewrap/Base/Base.hpp"
#include "Bubblewrap/Render/Types.hpp"
#include "Bubblewrap/Math/Vector2.hpp"
#include "Bubblewrap/Base.hpp"
#include "Bubblewrap/Render.hpp"

#include <memory>
#include <cstring>

namespace Bubblewrap
{
	namespace Render
	{
		Vertices::Vertices()
		{
			VertexCount_ = 0;
			Dirty_ = false;
			Texture_ = nullptr;
		}


		Vertices::~Vertices()
		{
		}

		void Vertices::Initialise( Json::Value Params )
		{
			Drawable::Initialise( Params );

			SetPrimitiveType( Converts::PrimitiveFromString( Params[ "primitiveType" ].asString() ) );
			Reserve( Params[ "vertices" ].size() );
			int uCount = Params[ "vertices" ].size();
			VertexCount_ = uCount;
			for ( int Idx = 0; Idx < uCount; ++Idx )
			{
				Vertices_.push_back( Vertex() );
				Vertices_[ Idx ].Colour_ = Colour( Params[ "vertices" ][ Idx ][ "colour" ].asString() );
				Vertices_[ Idx ].Position_ = Math::Vector3f( Params[ "vertices" ][ Idx ][ "position" ].asString() );
				Vertices_[ Idx ].TexCoords_ = Math::Vector2f( Params[ "vertices" ][ Idx ][ "texCoords" ].asString() );
			}
			OPTIONAL_LOAD( String, TextureName, texture );
		}


		void Vertices::Copy( Vertices* Target, Vertices* Base )
		{
			Drawable::Copy( Target, Base ); 
			Target->Reserve( Base->VertexCount_ );
			Target->Vertices_ = Base->Vertices_;
			Target->TextureName_ = Base->TextureName_;
			Target->PrimitiveType_ = Base->PrimitiveType_;
			Target->VertexCount_ = Base->VertexCount_;
			Target->Refresh();
		}

		void Vertices::Update( float dt )
		{
			Drawable::Update( dt );
			//AssertMessage( !Dirty_ , "Refresh has not been called"  );
			if ( ( Window_ == nullptr ) && ( WindowName_ != "" ) )
			{
				Window_ = GetManager().GetWindowManager().GetItem( WindowName_ );
			}
			if ( Window_ == nullptr )
				return;
			Refresh();

		}

		void Vertices::SetPrimitiveType( Primitives PrimitiveType )
		{
			PrimitiveType_ = PrimitiveType;
		}

		int Vertices::AddVertex( Vertex V )
		{
			Vertices_.push_back( V );
			++VertexCount_;
			Dirty_ = true;
			return VertexCount_ - 1;
		}
		void Vertices::Reserve( unsigned int Amount )
		{
			Vertices_.reserve( Amount );
		}
		void Vertices::SetVertex( unsigned int Idx, Vertex V )
		{
			assert( Idx < VertexCount_ );
			Vertices_[ Idx ] = V;
			Dirty_ = true;
		}

		void Vertices::Refresh()
		{

		}
		Vertex Vertices::GetVertex( int Idx )
		{
			return Vertices_[ Idx ];
		}

		Vertex &Vertices::EditVertex( int Idx )
		{
			Dirty_ = true;
			return Vertices_[ Idx ];
		}

		void Vertices::OnAttach()
		{
			if ( TextureName_ != "" )
				Texture_ = dynamic_cast<Texture*>( GetRegister().GetResource( TextureName_ ) );
		}

		void Vertices::SetDirty()
		{
			Dirty_ = true;
		}
	}
}