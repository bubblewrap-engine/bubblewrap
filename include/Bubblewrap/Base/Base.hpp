#ifndef GoBase_H
#define GoBase_H

#include "Bubblewrap/Base/EvtMessage.hpp"
#include "Bubblewrap/Managers/MgrManagers.hpp"
#include "Json/Json.h"
namespace Bubblewrap
{
	namespace Base
	{
		class ObjectRegister;
		class GoEntity;

		class GoBase
		{
			friend ObjectRegister;
		public:
			GoBase( );

			virtual void Update( float dt ) = 0;
			virtual void ReceiveMessage( EvtMessage Message );
			void Destroy();
			int Id();

			ObjectRegister & GetRegister();

			Managers::MgrManagers& GetManager();
			virtual void OnAttach();
			virtual void OnDetach();

			virtual void Initialise( Json::Value Params );

			GoEntity* GetParentEntity();
			std::string GetName();

			virtual std::string TypeName() = 0;
		private:

			GoEntity* Parent_;
			int Id_;
			bool Destroy_;
			ObjectRegister* ObjectRegister_;
			GoBase* Basis_;

			Managers::MgrManagers* Manager_;

			std::string Name_;
		};
	}
}

#endif