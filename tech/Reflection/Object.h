/****************************************************************************
This source file is (c) Teardrop Games LLC. All rights reserved.
Redistribution and/or reproduction, in whole or in part, without prior
written permission of a duly authorized representative of Teardrop Games LLC
is prohibited.
****************************************************************************/

#if !defined(OBJECT_INCLUDED)
#define OBJECT_INCLUDED

#include "Memory/Allocators.h"
#include "Util/_String.h"
#include "Util/UUID.h"
#include "Util/Event.h"

namespace Teardrop
{
    namespace Reflection
    {
        class ClassDef;
        class PropertyDef;

        /*
            Base object in reflective object hierarchy
        */
        class Object
        {
            UUID mId;

        public:
            Object();
            virtual ~Object();

            /*
                Initialize this object -- perform derived initialization
                in an override of this method and not in constructor. Note
                that in order to ensure proper initialization of the hierarchy,
                you need to call the base class initialize() method in all
                derived classes (usually call it as the first code in your
                initialize() override).
            */
            virtual bool initialize();

            /*
                Destroy this object -- perform derived destruction
                in an override of this method and not in destructor. Note
                that in order to ensure proper destruction of the hierarchy,
                you need to call the base class destroy() method in all
                derived classes (usually call it as the final code in your
                destroy() override).
            */
            virtual bool destroy();

            /*
                Object ID accessor/mutator. Typically, you do not call the
                mutator from derived or client code -- the object serialization
                framework is the only thing that should be calling the mutator.
            */
            const UUID& getObjectId();
            void setObjectId(const UUID& objectId);
            void setObjectId(const String& objectId);

            /*
                These are provided as hooks into derived classes, but without the
                need to make Object itself partake in reflection (makes sense, sorta)
            */
            virtual const ClassDef* getDerivedClassDef() const;

            /*
                This method is intended to allow derived classes to provide their own
                String representation of themselves (useful in editors, on-screen name tags,
                and so on). The return is intentionally by-value so as not to restrict
                how derived classes might create their string value -- if you prefer not to
                pass String objects by value, use the in-out version instead.
            */
            virtual String toString() const;
            virtual void toString(/*inout*/ String& str) const;

            void setupPropertyDefaults();

            virtual Object* clone() const;

            TD_DECLARE_ALLOCATOR();

            /*
                This sort of hacky way to do this, is here so that we do not have to couple
                directly to a method by which property change events can be broadcast; instead,
                if the Reflection library is used with such an event broadcast/message-passing
                system, then it can set us up with a pointer to a function that will create and
                post such a message for us.
            */
            typedef void (*BroadcastPropertyChanged)(Object* pObj, const PropertyDef* pPropDef);
            static void setBroadcastPropertyChangedFn(BroadcastPropertyChanged fn);

            /*
                These methods are the root of the "property changed" event/notification
                system. When a property is changed (using the get/set methods for it),
                it will call this method, which in turn will call an overrideable method
                as well as notifying the wider system. This allows for both non-broadcast
                notifications (so that an object can take action without having to subscribe
                to some round-trip notification/bulletin-board system to find out about
                changes in its own data), as well providing those broadcast notifications
                for systems and objects external to ourselves. If you derive from Object and
                want to handle change notifications for various of your properties, implement
                the notifyPropertyChangedImpl() method in your derived class.
            */
        public:
            void notifyPropertyChanging(const PropertyDef* pPropDef);
            void notifyPropertyChanged(const PropertyDef* pPropDef);
            // called after object instance created, and after property default have been set
            virtual void onInstanceCreated();

        protected:
            virtual void notifyPropertyChangingLocal(const PropertyDef* pPropDef);
            virtual void notifyPropertyChangedLocal(const PropertyDef* pPropDef);
            //static BroadcastPropertyChanged s_propChangedFn;

        public:
            // the preferred way to get notifications about property changes
            //fastdelegate::FastDelegate1<const PropertyDef*> PropertyChanged;
            Event<const PropertyDef*> PropertyChanging;
            Event<const PropertyDef*> PropertyChanged;

        private:
        };
    } // namespace Reflection
} // namespace Teardrop

#endif // OBJECT_INCLUDED
