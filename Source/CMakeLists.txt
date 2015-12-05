set(Base_SOURCES
	Application.cpp
	FrameStats.cpp
	GameState.cpp
	InputSystem.cpp
	IntroState.cpp
	MenuState.cpp
	Settings.cpp
	StateMachine.cpp
)

set(Base_HEADERS
	Application.hpp
	FrameStats.hpp
	GameState.hpp
	InputSystem.hpp
	IntroState.hpp
	MenuState.hpp
	Settings.hpp
	StateMachine.hpp
)

set(Game_SOURCES
	Game/Building.cpp
	Game/Ground.cpp
	Game/Inventory.cpp
	Game/Ladder.cpp
	Game/Ore.cpp
	Game/Player.cpp
	Game/QuadTree.cpp
	Game/Recipes.cpp
	Game/Tree.cpp
	Game/World.cpp
)

set(Game_HEADERS
	Game/Actor.hpp
	Game/Building.hpp
	Game/Ground.hpp
	Game/Inventory.hpp
	Game/Item.hpp
	Game/Ladder.hpp
	Game/Ore.hpp
	Game/Player.hpp
	Game/QuadTree.hpp
	Game/Recipes.hpp
	Game/Tree.hpp
	Game/World.hpp
)

set(Item_SOURCES
	Game/Items/ConstructionKit.cpp
	Game/Items/MinedOre.cpp
	Game/Items/Sapling.cpp
	Game/Items/Tool.cpp
	Game/Items/Wood.cpp	
)

set(Item_HEADERS
	Game/Items/ConstructionKit.hpp
	Game/Items/MinedOre.hpp
	Game/Items/Sapling.hpp
	Game/Items/Tool.hpp
	Game/Items/Wood.hpp	
)

set(Menu_SOURCES
	Menu/InGameMenu.cpp
	Menu/KeybindingPage.cpp
	Menu/MainMenu.cpp
	Menu/MenuPage.cpp
	Menu/OptionsPage.cpp
)

set(Menu_HEADERS
	Menu/InGameMenu.hpp
	Menu/KeybindingPage.hpp
	Menu/MainMenu.hpp
	Menu/MenuPage.hpp
	Menu/OptionsPage.hpp
)

set(Util_SOURCES
	Util/Animation.cpp
	Util/INI.cpp
	Util/OBJLoader.cpp
	Util/Resources.cpp
	Util/ShapeDraw.cpp
	Util/SpriteSheet.cpp
)

if (WIN32)
	set(Util_SOURCES ${Util_SOURCES} Util/FontFinder_Windows.cpp)
else()
	find_path(Fontconfig_INCLUDE_DIR fontconfig/fontconfig.h)
	find_library(Fontconfig_LIBRARY NAMES fontconfig)

	include_directories(
		${Fontconfig_INCLUDE_DIR}
	)

	set(Util_SOURCES ${Util_SOURCES} Util/FontFinder_Fontconfig.cpp)
endif()

set(Util_HEADERS
	Util/Animation.hpp
	Util/FontFinder.hpp
	Util/INI.hpp
	Util/OBJLoader.hpp
	Util/Resources.hpp
	Util/ShapeDraw.hpp
	Util/SpriteSheet.hpp
)

source_group(Base FILES ${Base_SOURCES} ${Base_HEADERS})
source_group(Game FILES ${Game_SOURCES} ${Game_HEADERS})
source_group(Game\\Items FILES ${Item_SOURCES} ${Item_HEADERS})
source_group(Menu FILES ${Menu_SOURCES} ${Menu_HEADERS})
source_group(Util FILES ${Util_SOURCES} ${Util_HEADERS})

add_executable(LD29 main.cpp ${Base_SOURCES} ${Base_HEADERS} ${Game_SOURCES} ${Game_HEADERS} ${Item_SOURCES} ${Item_HEADERS} ${Menu_SOURCES} ${Menu_HEADERS} ${Util_SOURCES} ${Util_HEADERS})
if (WIN32)
	target_link_libraries(LD29 ${SFML_LIBRARIES} opengl32 glu32)
else()
	target_link_libraries(LD29 ${SFML_LIBRARIES} ${Fontconfig_LIBRARY} GLU GL)
endif()
