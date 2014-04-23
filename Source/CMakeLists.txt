set(Base_SOURCES
	Application.cpp
	GameState.cpp
	InputSystem.cpp
	IntroState.cpp
	LoadingState.cpp
	MenuState.cpp
	Settings.cpp
	StateMachine.cpp
)

set(Base_HEADERS
	Application.hpp
	GameState.hpp
	InputSystem.hpp
	IntroState.hpp
	LoadingState.hpp
	MenuState.hpp
	Settings.hpp
	StateMachine.hpp
)

set(Game_SOURCES

)

set(Game_HEADERS

)

set(Menu_SOURCES
	Menu/InGameMenu.cpp
	Menu/MainMenu.cpp
)

set(Menu_HEADERS
	Menu/InGameMenu.hpp
	Menu/MainMenu.hpp
)

set(Util_SOURCES
	Util/Animation.cpp
	Util/JSON.cpp
	Util/QuadTree.cpp
	Util/SpriteSheet.cpp
)

set(Util_HEADERS
	Util/Animation.hpp
	Util/JSON.hpp
	Util/QuadTree.hpp
	Util/SpriteSheet.hpp
)

source_group(Base FILES ${Base_SOURCES} ${Base_HEADERS})
source_group(Game FILES ${Game_SOURCES} ${Game_HEADERS})
source_group(Menu FILES ${Menu_SOURCES} ${Menu_HEADERS})
source_group(Util FILES ${Util_SOURCES} ${Util_HEADERS})

add_executable(LD29 main.cpp ${Base_SOURCES} ${Base_HEADERS} ${Game_SOURCES} ${Game_HEADERS} ${Menu_SOURCES} ${Menu_HEADERS} ${Util_SOURCES} ${Util_HEADERS})
target_link_libraries(LD29 ${SFML_LIBRARIES})
