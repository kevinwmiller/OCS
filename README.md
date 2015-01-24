#OCS - ObjectComponentSystem
##Last Modified: 01/24/2014
##Version 1.0

OCS provides a fast and easy to use interface for creating, 
testing, and modifying objects, their components, and the systems that allow
everything to interact.  

===================================================================================================================

##ABOUT:

OCS was originally designed for a networked zombie game (<https://github.com/ayebear/undeadmmo>) created
by a few friends for both a learning experience and for some fun. We came across entity-component-systems
and were intrigued by the idea because of how different is was from object oriented design. The simplicity
of creating new entities, coupled with the efficient use of the cache was an interesting concept and knew it
could speed up our development time as well as reduce the amount of debugging we would have to do later on.
Working on this library has proven to be an invaluable learning experience and I hope that someone else will
find it useful.

Although OCS was designed for the above game, it is generic enough that it can be plugged into almost any kind
of project. Custom components, systems, and messages are very simple to create, and the interface is intuitive
enough to allow for an easy going experience. This library will continue to improve as more bugs are found and
more features are needed, so keep an eye out for any updates.

===================================================================================================================

##REQUIREMENTS:

*A compiler with c++11 support. This software was built and tested using gcc 4.7.
*Cmake version >= 2.8. Can be downloaded from <http://www.cmake.org/>

===================================================================================================================

##INSTALLATION:

1.    Open a command prompt in the root directory
2.    Create a build directory if it does not already exist

        mkdir build

3.    Navigate to build directory

        cd build

4.    Run cmake using the root as the source directory  
      4.1    To generate default makefile:

        cmake ..

    4.2    To generate MinGW makefiles:

        cmake -G "MinGW Makefiles" ..

    4.3    To see a list of other generators:

        cmake 

5.    Use the generated make file  
    5.1 Default makefile

        make install

    5.2    MinGW makefile

        mingw32-make install


===================================================================================================================

##SUPPORT:
	
If any bugs are found or if you have any suggestions for new features or changes, you can reach me at KevM1227@gmail.com.

For more information, see the project's home page at https://github.com/Lastresort92/OCS
	
===================================================================================================================
			
##LICENSING:

OCS is licensed under the zlib license. See LICENSE.txt for full details.

===================================================================================================================

##CHANGE LOG:

-	v1.0
    -	  Added functionality for components
    -	  Added functionality for systems
    -	  Added functionality for messages
    -	  Added basic functionality for states
    -	  Added functionality for prototypes

===================================================================================================================

##UPCOMING CHANGES:

-	Add queues for creating and destroying objects/components (Fix for PackedArray reserving a lot of space up front)
-	Add ability to use different format for prototype files
-	Add ability for Serializer to be able to serialize to binary format
-	Add ability for Serializer to be able to serialize maps, arrays, and other containers
-	Finish States

===================================================================================================================

##KNOWN BUGS:

Program will crash if wrong format is used when loading prototypes.

===================================================================================================================

##INSTRUCTIONS:

There are four main parts to OCS: components, objects, systems, and messaging. The following sections will review all of these and show examples on how to use them.
	
===================================================================================================================	

*To jump to a section, highlight the title and press ctrl-c, ctrl-f, ctrl-v.*
	
##TABLE OF CONTENTS:
	
*	COMPONENTS
	*	CREATING A COMPONENT
	*	SERIALIZING A COMPONENT
*	OBJECTS
	*	CREATING OBJECTS
		*	CREATING A BLANK OBJECT
		*	CREATING AN OBJECT WITH COMPONENTS
		*	CREATING A COMPONENT FROM A PROTOTYPE
	*	ADDING COMPONENTS
	*	REMOVING COMPONENTS
	*	DESTROYING AN OBJECT
	*	QUERYING AN OBJECT'S COMPONENTS
	*	OBJECT PROTOTYPES
		*	CREATING A PROTOTYPE IN THE SOURCE CODE
		*	CREATING A PROTOTYPE IN AN EXTERNAL FILE
		*	LOADING PROTOTYPES
		*	PROTOTYPE SETS
		*	LOADING A PROTOTYPE SET		
*	SYSTEMS
	*	CREATING A SYSTEM
	*	ADDING SYSTEMS
	*	REMOVING SYSTEMS
	*	UPDATING SYSTEMS
	*	WRITING THE UPDATE FUNCTION
*	MESSAGING
	*	TRANSCEIVERS
	*	MESSAGES
		*	CREATING MESSAGES
	*	LOGGING MESSAGES
		*	LOGGING A SINGLE MESSAGE
		*	LOGGING ALL POSTED MESSAGES
		*	LOGGING ALL PRIVATE MESSAGES
	*	SENDING MESSAGES
		*	SENDING POSTING MESSAGES
		*	SENDING PRIVATE MESSAGES
	*	READING MESSAGES
		*	READING POSTED MESSAGES
		*	READING PRIVATES MESSAGES
	*	CLEARING MESSAGES
		*	CLEARING POSTED MESSAGES
		*	CLEARING PRIVATE MESSAGES
*	TIPS

===================================================================================================================
			
##COMPONENTS:
		
The component is the basis of OCS. They define what objects are and how they interact with
the rest of the world. A component in OCS is a POD type, and represents a single aspect
about an object. All components are stored in arrays to limit the amount of cache misses that
occur in a program. Components and Messages make use of the Curiously Recurring Template Pattern(CRTP)
to keep track of their ids.

**NOTE: No logic other than serializing or deSerializing should be in the components and all components 
MUST implement a default constructor and SHOULD implement a parameratized constructor.**
	
		
###CREATING A COMPONENT:

A Position component might have 2 float values: an x, and a y. To create this component, you might
make it like this.
	
	struct Position : public ocs::Component<Position>
	{
		Position(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) {}
		
		float x, y;
	};
	
This component can then be added to an object using the ObjectManager class.
	
###SERIALIZING A COMPONENT*:

All components have access to a Serializer object. This object operates in a similar way to printf and scanf in 
how it takes a format string and a list of variables, and returns a string in the given format.

To allow a component to be serialized or deserialized (perhaps to be loaded from a file), simply overload the serialize
and deSerialize functions in your component. Then in the definition, call the serialize or deSerialize function of the 
Serializer object, and give it a format string and a list of values to serialize.



	struct Position : ocs::Component<Position>
	{
		Position(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) {}
		
		std::string serialize() { return serializer.serialize("% %", x, y); }
		void deSerialize(const std::string& values) { serializer.deSerialize("% %", values, x, y; }
		
		float x, y;
	};
		
The component can then be converted to and from a string by calling the serialize or deSerialize functions.

*More serializing options are coming soon. Converting to and from byte arrays, and loading arrays into the string are next additions.

===================================================================================================================
	
##OBJECTS:

An object in OCS is represented by an ID and is the element that ties a group of components together. The
ObjectManager retrieves the object's id and can use that to locate a specific component in the component's
array.

###CREATING OBJECTS:

*NOTE: All examples below assume an instance of ObjectManager called objManager exists. If your manager inherits from State, one is given to you.*
	  
Objects can be created in three different ways: as an empty object with no components, an object with one or more components passed
to the createObject function, or copied from an object prototype.

####CREATING A BLANK OBJECT:

	auto id = objManager.createObject();

Creating an empty object should be rarely used, but if you do then it is recommended that you store the id of the 
object in order to add components on set up. 

####CREATING AN OBJECT WITH COMPONENTS:

Assuming components called "Position", "Velocity", and "Renderable" have been created.

	objManager.createObject( Position(45, 75), Velocity(34, 4), Renderable("Some texture") );

####CREATING AN OBJECT FROM A PROTOTYPE:

To create objects using a prototype, pass in the name of the prototype. To create 10 objects from
a prototype called "Player" you might do this:

	for(int i = 0; i < 10; ++i)
		objManager.createObject("Player");

All objects created will have the same components with the same values that were given to the prototype on
creation.

####ADDING COMPONENTS:

If a component needs to be added to an object after creation you can do so my calling the addComponents function.

  
	objManager.addComponents( Position(65, 45) );

Multiple components can be added at once. However, keep in mind that an object may only have one instance of each type
of component for technical reasons.

####REMOVING COMPONENTS:

To remove one or more components from an object, call the removeComponents function.


	objManager.removeComponents<Position, Velocity>(45);

####DESTROYING AN OBJECT:

The following code will destory an object with the id 4 as well as all of its components.

	objManager.destroyObject(4);

####QUERYING AN OBJECT'S COMPONENTS

To determine if the object 45 has Position, Velocity, and Renderable components use the following code.

	objManager.hasComponents<Position, Velocity, Renderable>(45);

Returns true if the object has all components and returns false if the object is missing at least one of the components.

####OBJECT PROTOTYPES

Object prototypes can make creating new objects much cleaner. Instead of specifying the types and values
of components every time a new object is needed, you can define what components an object should have and
give it an alias to refer by later. Prototypes can be made directly in the source code or in a separate file.

####CREATING A PROTOTYPE IN THE SOURCE CODE

To create an object prototype, call the addComponentsToPrototype function in ObjectManager and specify the
name of the prototype to add the components to. If the prototype does not already exist, one will be created.

	objManager.addComponentsToPrototype("Player", Position(65, 45), Velocity(200, .5), Renderable("Txtre File.png");

This will create a prototype under the name "Player" with Position, Velocity, and Renderable components.

Any objects that are created from this prototype will be given the same components with the initial values.

####CREATING PROTOTYPES IN AN EXTERNAL FILE

The ObjectPrototypeLoader class allows you to easily create and test new types of objects by defining them in
an external file. Defining the prototypes in a file prevents the program from recompiling whenever a change
to a prototype is made. Multiple prototypes are able to be loaded in one function call to keep the source code clean.

#####REQUIREMENTS:

-	All used components must implement a deSerialize function.
-	The values for the components must be in the format that was specified in the deSerialize function.
-	All used components must be bound to a string so the prototype loader can determine the type of the component.
	objManager.bindStringToComponent<Position>("Position");

A prototype in a file has three main parts:
-	The Section header which is the name of the prototype.
-	An array of component names
-	The values of each component.

Every section must have a section header and an end tag. These markers are enclosed in square brackets('[' , '/]').

A prototype called "Player" would have a section that looks like this.

	[Player]
	
	[/Player]

The component names and values would go in between these markers.

Every prototype section should have an attribute call "Components". This attribute will contain an array
of component name that define what components the prototype has. All components and attribute are enclosed
in double quotes, and an array is a comma separated list of attributes enclosed in curly braces ('{' , '}').

A colon separates an attribute name from its values.

	[Player]
		
		"Components" : { "Position", "Velocity", "Renderable" }
		
	[/Player]

Each component name should then have an attribute with the same name as the one that is in the list.
Each attribute will then have an a single attribute, a string in the format that was specified in the serialize function.

Assuming Position and Velocity take the format "% %", and Renderable takes the format "%s".
String values should be enclosed in single quotes.

	[Player]
		
		"Components" : { "Position", "Velocity", "Renderable" }
		
		"Position" : "65 45"
		"Velocity" : "200 .5"
		"Renderable" : "'Txtre File.png'"
		
	[/Player]

After this prototype is loaded, it can be used as if it was hard coded. There is no performance difference after the
initial set up.

#####LOADING PROTOTYPES

Before loading a prototype, you must bind your components to a string.

Somewhere before we load our player prototype, we must bind the Position component to the string "Position",
the Velocity component to the string "Velocity", and the Renderable component to the string "Renderable".
The associated words do not need to be named the same as the component. It is just recommended to keep
everything consistent.

	//In the configure function of the State class, or somewhere else.
	objManager.bindStringToComponent<Position>("Position");
	objManager.bindStringToComponent<Velocity>("Velocity");
	objManager.bindStringToComponent<Renderable>("Renderable");

Now we can load our player prototype.

The loadObjectPrototype function is static within ObjectPrototypeLoader.
It takes an ObjectManager reference, a file path, and the name of the prototype as parameters.

	//Assuming our player prototype is in "player.txt" and it is in the root folder.
	ObjectPrototypeLoader::loadObjectPrototype(objManager, "player.txt", "Player");

The "Player" prototype can then be used to create objects.

Please note that there may be more than one prototype per file, but each one must have a unique name.

#####PROTOTYPE SETS

A prototype set is a list of prototypes that can simplify the loading of prototypes.

To load a prototype set, create a section in the same manner as above, and create an attribute
with a value that contains an array of the prototype names.

	[Set]					
		"Prototype Set" : { "Player", "Ball", "Brick" }					
	[/Set]
	
Then somewhere else in the file would be the definitions of the prototypes. These are defined the same way
as in the previous section.

#####LOADING A PROTOTYPE SET

Like the loadObjectPrototype function, loadPrototypeSet is static within ObjectPrototypeLoader.

This function takes an ObjectManager reference, the file with the prototype set and definitions,
the name of the prototype set attribute, and the section that this attribute resides in.

	//Assuming the above example is in prototypes.txt and all prototypes are defined
	ObjectPrototypeLoader::loadPrototypeSet(objManager, "prototypes.txt", "Prototype Set", "[Set]");
	
Notice the square brackets around the name of the section.

Calling this function would be the same as calling loadObjectPrototype for all 3 prototype names.

		

===================================================================================================================

##SYSTEMS:

The core of a system is a single update function that is called every frame. This is the logic of the program that allows components to
interact. A system usually operates on a single component's array and would fetch needed information from the ObjectManager.

###CREATING A SYSTEM:
To create a system, simply inherit from the System class and implement an update function that takes an ObjectManager reference,
a MessageHub reference, and a double for the elapsed time.

The following code implements a movement system that operates on objects with Velocity components. This assumes that velocity has
two floats called "speed" and "angle".

	struct MovementSystem : public ocs::System
	{
		void update(ObjectManager& objManager, MessageHub& msgHub, double dt)
		{
			//Iterate through ALL velocity components. Notice we are taking in a reference. We do this so we can
			//modify the actual component.
			for( auto& vel : objManager.getComponentArray<Velocity>() )
			{
				//Use the velocity component's owner id to get the object's position component
				auto pos = objManager.getComponent<Position>( vel.getOwnerID() );
				
				//Check if the object actually had a Position component.
				if(pos)
				{
					Use the velocity component to update the object's position.
					pos->x += cos(vel.angle) * vel.speed * dt;
					pos->y += sin(vel.angle) * vel.speed * dt;
				}
			}
		}
	};

Systems can use the MessageHub to post messages if anything interesting occurs (Such as a collision). See Messaging section.

###ADDING SYSTEMS:

NOTE: Each SystemManager can have one instance of each type of system for technical reasons. (Why would you need more?)

Assuming a system call MovementSystem has been created, and a SystemManager called sysManager is available,
add it to the SystemManager thusly:

	sysManager.addSystem<MovementSystem>();

If systems are updated using the updateAllSystems function in the SystemManager, then systems will be updated in the
order that they were added in. A system can be manually updated by called the updateSystem function and passing in the
system as the template paramater.

	sysManager.updateSystem<MovementSystem>(dt);

###REMOVING SYSTEMS:

Removing Systems is as easy as passing in the system type to the removeSystem function.

	sysManager.removeSystem<MovementSystem>();
	
###UPDATING SYSTEMS:

All systems are, in essence, a single update function. This function should operate a single component array and be as
decoupled as possible. Every update function has access to an ObjectManager, MessageHub, and the time elapsed from the
previous frame.

Systems can be updated in two ways: update all systems in a single function call, or individually.

If updateAllSystems is called, then the systems will be updated in the order that they were added in.
	
	sysManager.updateAllSystems(dt);
	
You can also call updateSystem and pass in the System type as a template paramater to manually update a system.

	sysManager.updateSystem<MovementSystem>(dt);
	
####WRITING THE UPDATE FUNCTION:

An update function for a movement system might look like this:
	
	//Assuming components call Velocity(an angle and speed) and Position(an x and a y) have been created
	void MovementSystem::update(ObjectManager& objManager, MessageHub& msgHub, double dt)
	{
		for(auto& vel : objManager.getComponentArray<Velocity>())
		{
			//Attempt to get the velocity's owner's position component
			auto pos = objManager.getComponent<Position>(vel.getOwnerID);
			
			//Ensure that the object has a position component. 
			if(pos)
			{
				pos->x += cos(vel.angle) * vel.speed * dt;
				pos->y += sin(vel.angle) * vel.speed * dt;
			}
		}
	}

===================================================================================================================	

##MESSAGING:

###TRANSCEIVERS:

A transceiver in this context is anything that can send or receive messages through the MessageHub.

By default, all States and systems are transceivers. If there is anything else that you need to use
the MessageHub, you can make that object inherit from the Transceiver struct.

To use the MessageHub, you must have access to a Transceiver object, either through inheritance or composition.
Inheriting from Transceiver allows the creator to pass "*this" to the message functions. However, there is no
difference otherwise.

###MESSAGES:

A message is similar to a component in that it is a POD type. However, messages can be posted such that
it is available for all other transceivers to see. A message can also be sent privately to another transceiver
if the receiving ID is known.

####CREATING MESSAGES:

Creating Messages is very similar to creating a component. Simply inherit from "Component" and pass in the new class type as
a template paramater.

Give the message a constructor that takes a const Transceiver reference, which will be the message creator, and any
other paramaters that are needed to set up the message.

All messages must take a const Transceiver reference an pass the transceiver to the Message base class constructor.



	struct ObjectDestroyed : public Message<ObjectDestroyed
	{
		ObjectDestroyed(const Transceiver& transceiver, ocs::ID _destroyedObjectID) : 
			Message(transceiver), destroyedObjectID(_destroyedObjectID) {}
		
		ocs::ID destroyedObjectID;
	};
	
The message can then be posted to be viewed publically or it can be sent in a private message.

####LOGGING MESSAGES:
	
Messages can be logged to any ostream object such as the console window or an open file.

All messages have default logging which contains the Message Family, and the sender id. To gain access to more
specialized logging overload the "log" function in your message object and define how the message should be logged.

	struct ObjectDestroyed : public Message<ObjectDestroyed
	{
		ObjectDestroyed(const Transceiver& transceiver, ocs::ID _destroyedObjectID) : 
			Message(transceiver), destroyedObjectID(_destroyedObjectID) {}
		
		void log(std::ostream& out)
		{
			out << "Message Type: ObjectDestroyed\n";
			out << "Message Family: " << getFamily() << std::endl;
			out << "Sender: " << getSender() << std::endl;
			out << "Destroyed Object ID: " << destroyedObjectID << std::endl;
		}
		
		ocs::ID destroyedObjectID;
	};
	
Ideally, the logged message should include the Message Type(The name of the message object), the message family(The family id
that was assigned to the message), the transceiver id of the sender, and all appropriate information about the message, all with labels.

#####LOGGING A SINGLE MESSAGE:
	
To log a single message, call the log function on an instantiated message object and call log and pass in an ostream object.

	ObjectDestroyed objDestroyed(*this, 5);
	objDestroyed.log(std::cout);

Messages will rarely be created in this manner as the MessageHub should handle all message creation.

#####LOGGING ALL POSTED MESSAGES:

	To log all messages that are currently posted in the MessageHub, call logPostedMessages and pass in an ostream object.

		msgHub.logPostedMessages(std::cout);
	
#####LOGGING ALL PRIVATE MESSAGES:

	To log all of a transceiver's private messages, call logPrivateMessages and pass in the transceiver object and an ostream object.
	
		msgHub.logPrivateMessages(*this, std::cout);

####SENDING MESSAGES:

#####SENDING POSTING MESSAGES:

Messages are able to be posted publically for all transceivers to view. To post a message, pass in the message type as
a template paramater, and then pass in the creating transceiver and any paramaters for the message.

 Assuming a message called ObjectDestroyed has been created, the poster inherits from Transceiver, and a MessageHub called
msgHub is available.
	
	//Post that object with id '5' has been destroyed
	msgHub.postMessage<ObjectDestroyed>(*this, 5);

The message hub will then create a new ObjectDestroyed object with the given paramaters, and other Transceivers can view it
by using the readPostedMessages function.
	
#####SENDING PRIVATE MESSAGES

All transceivers are given access to a "mailbox" that other transceivers can send messages to. Sending private messages
is nearly identical to posting a message. However, you must know the transceiver id of the receiver.

The following code assumes that a "TextMessage" message has been defined that takes a transceiver object and a single string argument.
Also assumes that the sender inherits from "Transceiver".

	//Send a TextMessage to transceiver with id '4'
	msgHub.sendPrivateMessage(4, *this, "Hello Transceiver 4!");

####READING MESSAGES:

#####READING POSTED MESSAGES:

The MessageHub acts as a bulletin board that is organized by the types of the messages. All transceivers can view these posted messages.

To get a list of all messages of a certain type, pass in the message type as a template paramater to the readPostedMessages function.

	//Get a list of all TextMessage messages and store them in textMsgs.
	auto textMsgs = msgHub.readPostedMessages<TextMessage>();
	
Returns a vector of all posted messages of the given type. It is recommended to create a variable using auto as the return type
may change in future versions (i.e. vector -> list etc.).

Reading posted messages has no effect on the actual message board.
	
#####READING PRIVATE MESSAGES:

Reading private messages is nearly the same as reading posted messages. The only difference is you give the function your
Transceiver object to gain access to your mailbox.

	auto privateTxtMessages = msgHub.readPrivateMessages<TextMessage>(*this);

####CLEARING MESSAGES:

#####CLEARING POSTED MESSAGES:

If the message board is not cleared periodically, it can become cluttered and slow. Therefore, it is recommended to call
clearPostedMessages every frame in the main loop.

An automatic solution is in the works.

#####CLEARING PRIVATES MESSAGES:

If you wish to clear your personal mailbox, call clearPrivateMessages and pass in your Transceiver object.

##TIPS:

-	Post any user input to the MessageHub and allow the systems to determine what to do with it.
-	Use the new ranged based for loops to step through the component arrays. 
	-	for(auto& renderComp : objManager.getComponentArray<Renderable>())
-	Use the configure function in the State class to create any object prototypes.
-	Prefer to load prototypes from a file to allow for faster modifying of prototypes.
	-	The program will not need to be recompiled if an external file is modified.
-	Use the initialize function in the State class to create actual objects and set their starting position.
-	Call 'clearPostedMessages' for the message hub every frame to prevent messages from piling up.
-	Try to avoid creating systems that rely on other systems or components that rely on other components. The more decoupled everything is the better.
-	Email me anymore tips, suggestions, or bugs as you find them.
===================================================================================================================
