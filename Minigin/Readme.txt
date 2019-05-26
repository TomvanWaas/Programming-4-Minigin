//Github Link:
https://github.com/TomvanWaas/Programming-4-Minigin/tree/master/Minigin




//////////////////////////////////////////////////////////////////////////////
// Game Loop /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//*Game Loop design pattern

Engine.Run(...)
{
- Init Engine

while 
//Loop to be able to reload the application and scenemanager
//without using another if-statement in the second while and
//without reloading the engine itself

	- Init Application & Scenemanager

	while
	//While loop representing the game loop

		- Process Input
		//Is able to quit the loop when SDL_Quit event is generated or when it is notified to quit

		- Update SceneManager (UpdateFirst & UpdateSecond)
		//=> Update the active Scene

		- Fixed Update SceneManager (with lag count)
		//This happens in a while using lag, to prevent advantages of low fps
		//=> Fixed Update active Scene

		- Delete objects that were vital in loops
		//Objects can be stored to be deleted here, to prevent problems with loops 
		//or when eg: a GameObject destroys itself in its own Update...

		- Render
		//Render the SceneManager => Render the active Scene

	- Cleanup Application & SceneManager

- Cleanup Engine
}


////////////////
// Engine //////
////////////////
{
Has the Run(Application*, WindowSettings) function.
Static function to quit the game loop.
Static function to reload the application and sceneManager.
Static getter for the WindowSettings.
}


////////////////
// Application /
////////////////
(abstract)
{
Adds the Required Scenes to the SceneManager in its virtual 'Initialize(SceneManager, WindowSettings)' function.
}

//////////////////////////////////////////////////////////////////////////////
// SCENEGRAPH ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
{
Initialize: Initializes all Scenes
			Call virtual SceneInitialize function
			Initializes SceneData
			Initializes all root GameObjects
					Initializes all Components & Children
			InitializeLate SceneData

Update: Updates Active Scene
			Updates SceneData
			Call virtual SceneUpdate function
			Update(-First & -Second) all root GameObjects
					Update(-First & -Second) all Components & Children
UpdateFixed: UpdateFixed Active Scene
			UpdateFixed SceneData
			Call virtual SceneUpdateFixed function
			UpdateFixed all root GameObjects
					UpdateFixed all Components & Children
Render:	Render Active Scene
			Renders RenderManager containing registered all Renderables (Eg: RenderComponent)

}


////////////////////////////////
/// SceneManager ///////////////
////////////////////////////////
{
Manages the Scenes.
Initializes all, Updates active Scene and Renders Active Scene.
Contains Adders, Getters, Setters for the Scenes.
Deletes them in destructor
}


////////////////////////////////
/// Deletor ////////////////////
////////////////////////////////
{
Can be accessed globally (Singleton).
Stores objects that must be deleted later in template containers.
Deletes them after the SceneManager is updated to prevent problems.
}


////////////////////////////////
/// Scene //////////////////////
////////////////////////////////
(can be derived from)
{
Manages its GameObjects. (only root GameObjects are registered)
Contains virtual functions to enable Application specific Scenes.
Contains SceneData.
Can acces its SceneManager owner
Deletes GameObjects in destructor
}


////////////////////////////////
/// SceneData //////////////////
////////////////////////////////
{
Contains Managers
Eg: Time, CollisionManager, RenderManager, InputManager
to be able to (un)register objects to the their managers.
Contains the possibility to add/get application specific Managers (abstract Manager class)
Deletes the managers in destructor
}


////////////////////////////////
/// GameObject /////////////////
////////////////////////////////
{
Manages its Components (Add, Remove, Delete)
Manages its children (Create, Add, Remove, Delete)
Can acces its Scene owner
Can have a parent
Is both a subject and an observer.
Has a Transform
Deletes components and children in destructor
}


////////////////////////////////
/// BaseComponent///////////////
////////////////////////////////
(abstract)
{
Contains Initialize, UpdateFirst, UpdateSecond, UpdateFixed and Destroy functions
Can acces its GameObject owner.
Is both a subject and an observer.
}



////////////////////////////////
/// RenderManager///////////////
////////////////////////////////
{
Renders Renderables (= abstract class) (Eg: RenderComponent)
Part of SceneData
Renderables can use the RenderManager to render Textures or shapes
}


////////////////////////////////
/// ResourceManager ////////////
////////////////////////////////
{
Singleton
Contains a vector of Loaders (= abstract)
These loaders are casted to a ResourceLoader<T> object when something needs to be loaded
ResourceLoader<T> can be specialized (Eg: TextureLoader & FontLoader are in Engine)
Best practice is to keep track of the loaded resources to prevent double loads
Basepath can be set using 'SetPath(path)'
}



////////////////////////////////
/// InputManager ///////////////
////////////////////////////////
{
//Based on the way the input works in the engine we use for Graphics Programming 2
//Commands are based on the Command design pattern

Contains static current and previous state of keyboard and controllers
Contains static functions to process input (Static because all input would be the same)
Contains container of InputActions
	InputAction
	{
	Contains ids for the keys
	Contains shared_ptr to a Command that must be executed when the action is triggered
	State can be triggered on: Pressed, Released, Up, Down
	}
When SceneData gets updated, a function will be called to only process the inputactions of that scene.
}

////////////////////////////////
/// Observers & Subjects ///////
////////////////////////////////
{
// Based on the Observer design pattern

	Subjects
	{
	Contains vector of observers to notify
	Is able to Unregister observers if destroyed
	}

	Observer
	{
	Contains vector of subjects to unregister from when destroyed
	Is able to Unregister subjects if destroyed
	}
Warning, they do not manage eachother.
}


//////////////////////////////////////////////////////////////////////////////
// Components ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

////////////////////////////////
/// BaseComponent //////////////
////////////////////////////////
(abstract)
{
Contains a char representing its State (Constructed, enabled, destroyed)
Contains a pointer to its owner (GameObject)
Is both a Subject and an Observer
Has virtual protected Initialize, UpdateFirst, UpdateSecond, UpdateFixed, Destroy and Notify functions
They are called when their public variants are called and they have a valid State.
Managed by the GameObject
}

////////////////////////////////
/// Example: SpriteComponent ///
////////////////////////////////
{
Contains a shared_ptr to a Texture2D
Contains a map of key int and value SpriteSource* which it has ownership of.
	SpriteSource (abstract)
	{
	Types: 
	- TickSource (Changes source (Rect) every small timestamp)
	- SweepSource (Smoothly lerps its source (Rect) )
	- FixedSource (Has a fixed source (Rect) )
	- GrowSource (a TickSource which can have different source (Rect) sizes)
	- ... Can be derived from by the engine user to create its own SpriteSource
	Contains a function GetSource which returns a source (Rect) and takes parameters 'accuTime' and 'spriteSpeed'
	}
Time can be stopped/continued by freezing the SpriteComponent
}


////////////////////////////////
/// Example: RenderComponent ///
////////////////////////////////
{
Derives also from Renderable
Contains a shared_ptr to a Texture2D, a Vector2 pivot, 
a FlipMode and a source (Rect) and a boolean to tell if it has a source.
Renders the Texture with its values on the Transform of the GameObject it belongs to.
Contains Setters and Getters for its members.
}


/////////////////////////////////
/// Example: 			/
/// FiniteStateMachineComponent /
/////////////////////////////////
//*Based on the Finite State Machine design pattern
{
Contains a kind of blackboard called FSMData.
Manages its FSMStates.
Contains a pointer to its current active FSMState.
Contains getters and setters for its FSMStates.
Updates, Notifies ... its current active FSMState.

	FSMState (abstract)
	{
	Contains function for Initialize, Update(-First & -Second), UpdateFixed, Enter, Exit and Notify.
	Can access its FiniteStateMachineComponent it belongs to and so its GameObject and other FSMStates.
	Can change the State of the FiniteStateMachineComponent at any time.

	Has a default derived class
		FSMStateDefault
		{
		Shared_ptr to events and Transitions which are called on the corresponding functions. (Eg: UpdateFirstEvents, EnterEvents...)
			FSMTransition
			{
			Contains a Conditions and a State
			}
		Changes State if conditions are met (Transitions)
		}
	}

}



//////////////////////////////////////////////////////////////////////////////
// Templates /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

template <class T>
Singleton (abstract)
{
//*Singleton design pattern

Contains a static instance of T
Contains a private constructor
}

//Singleton is used for the Logger and the ResourceManager


template <class T, class N>
Locator (abstract)
{
//*Service Locator design pattern

T represents the base class of the Locator
N represents the default value of the Locator

}


//////////////////////////////////////////////////////////////////////////////
// How To Use ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

1. Make an Engine object.
	- Call its Run(...) function passing an Application pointer and a WindowSettings struct.

2. Application
	- Derive a class from Application
	- Initialize the SceneManager in its virtual Initialize function with some Scenes

3. Scenes
	- Derive from Scene or make a Scene object
	- Add some GameObjects with components to the Scene using the Adder/Creator function or if derived, in its SceneInitialize function.
	- Add some Managers to its SceneData, create your own Components...


