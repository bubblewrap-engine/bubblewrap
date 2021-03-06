#include "Bubblewrap/Base/Entity.hpp"
#include "Bubblewrap/Base/Component.hpp"
#include "Bubblewrap/Base/ObjectRegister.hpp"
#include "Bubblewrap/Logs/Log.hpp"
namespace Bubblewrap
{
	namespace Base
	{

		Entity::Entity()
		{

		}

		void Entity::Initialise( Json::Value Params )
		{
			Logs::Log log( "Entity::Initialise" + Params[ "name"].asString() );

			GoBase::Initialise( Params );
			if ( Params[ "position" ].isString() )
			{
				Position_ = Math::Vector3f( Params[ "position" ].asString() );
			}
			if ( Params[ "children" ].isArray() )
			{
				log.WriteLine( "Loading children", Logs::StaticLog::INFO );
				for ( unsigned int Idx = 0; Idx < Params[ "children" ].size(); ++Idx )
				{
					Children_.push_back( GetRegister().CreateObject( Params[ "children" ][ Idx ], this ) );
				}
				log.WriteLine( "Loaded children", Logs::StaticLog::INFO );
			}
			if ( Params[ "components" ].isArray() )
			{
				log.WriteLine( "Loading components", Logs::StaticLog::INFO );
				for ( unsigned int Idx = 0; Idx < Params[ "components" ].size(); ++Idx )
				{
					static_cast< Component* >( GetRegister().CreateObject( Params[ "components" ][ Idx ], this ) );
				}
				log.WriteLine( "Loaded components", Logs::StaticLog::INFO );
			}
		}

		void Entity::Update( float Dt )
		{
			unsigned int uCount = Components_.size();
			for ( unsigned int Idx = 0; Idx < uCount; ++Idx )
			{
				Components_[ Idx ]->Update( Dt );
			}

			uCount = Children_.size();
			for ( unsigned int Idx = 0; Idx < uCount; ++Idx )
			{
				Children_[ Idx ]->Update( Dt );
			}
		}

		void Entity::Copy( Entity* Target, Entity* Base )
		{
			GoBase::Copy( Target, Base );
			Logs::Log log( "Entity::Copy" );
			Target->Position_ = Base->Position_;
			unsigned int uCount = Base->Components_.size();
			for ( unsigned int Idx = 0; Idx < uCount; ++Idx )
			{
				static_cast<Component*>( Base->GetRegister().CreateCopy( Base->Components_[ Idx ], Target ) );
			}

			uCount = Base->Children_.size();
			for ( unsigned int Idx = 0; Idx < uCount; ++Idx )
			{
				Base->GetRegister().CreateCopy( Base->Children_[ Idx ], Target );
				// Children_[ Idx ]->Update( Dt );
			}

		}
		Math::Vector3f Entity::WorldPosition()
		{
			if ( GetParentEntity() == nullptr )
			{
				return LocalPosition();
			}
			return ( GetParentEntity() )->WorldPosition() + LocalPosition();
		}
		Math::Vector3f Entity::LocalPosition()
		{
			return Position_;
		}

		void Entity::SetLocalPosition( Math::Vector3f Position )
		{
			Position_ = Position;
		}

		void Entity::LogHierarchy()
		{
			Logs::Log log;
			log.WriteLine( GetName() + "{Entity}", Logs::StaticLog::INFO );
			{
				Logs::Log log2;

				for ( unsigned int Idx = 0; Idx < Components_.size(); ++Idx )
				{
					try
					{
						log2.WriteLine( Components_[ Idx ]->GetName() + "{" + Components_[ Idx ]->TypeName() + "}", Logs::StaticLog::INFO );
					}
					catch (int)
					{
					}/**/
				}
				for ( unsigned int Idx = 0; Idx < Children_.size(); ++Idx )
					( ( Entity* ) Children_[ Idx ] )->LogHierarchy();
			}
		}

		Entity* Entity::GetRootEntity()
		{
			if ( GetParentEntity() == nullptr )
				return this;
			else
				return GetParentEntity()->GetRootEntity();
		}


		void Entity::RemoveChild( GoBase* Child )
		{
			for ( std::vector < GoBase* >::iterator iter = Children_.begin(); iter != Children_.end(); )
			{
				if ( ( *iter ) == Child )
				{
					iter = Children_.erase(iter);
				}
				else
					++iter;
			}
			for ( std::vector < Component* >::iterator iter = Components_.begin(); iter != Components_.end(); )
			{
				if ( ( *iter ) == Child )
				{
					iter = Components_.erase( iter );
				}
				else
					++iter;
			}
		}

		void Entity::Destroy()
		{
			GoBase::Destroy();
			for ( unsigned int Idx = 0; Idx < Children_.size(); ++Idx )
			{
				Children_[ Idx ]->Destroy();
			}
			for ( unsigned int Idx = 0; Idx < Components_.size(); ++Idx )
			{
				Components_[ Idx ]->Destroy();
			}
		}
	}
}
