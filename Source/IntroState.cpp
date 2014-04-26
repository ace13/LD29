#include "IntroState.hpp"
#include "MenuState.hpp"
#include "Util/FontFinder.hpp"

#include "Util/QuadTree.hpp"

#include "Game/Actor.hpp"

#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

QuadTree tree(sf::FloatRect(0, 0, 1024, 768), 5);

class MouseActor : public Actor
{
public:
	MouseActor(sf::RenderWindow& win) : mWin(win) { }

	void update()
	{
		mPos = (sf::Vector2f)sf::Mouse::getPosition(mWin);
	}

	sf::Vector2f getPosition() const
	{
		return mPos;
	}

private:
	sf::Vector2f mPos;
	sf::RenderWindow& mWin;
};

MouseActor* temp = nullptr;

IntroState::IntroState() : mTime(0)
{
	mFont = FontFinder::findDefaultFont();
}

IntroState::~IntroState()
{

}

void IntroState::update(double dt)
{
	if (temp)
		temp->update();
	mTime += dt;

	tree.update(dt);

	if (mTime > 1)
	{
		auto stateKeeper = getStateMachine().curState();
		getStateMachine().popState();
		getStateMachine().pushState(new MenuState());
	}
}

void IntroState::draw(sf::RenderTarget& target)
{
	if (!temp)
	{
		temp = new MouseActor((sf::RenderWindow&)target);
		tree.addActor(temp);
	}

	tree.draw(target);

	sf::Text introText("<Intro placeholder>", mFont);
	{
		auto rect = introText.getLocalBounds();
		introText.setOrigin(rect.width / 2.f, rect.height / 2.f);
	}

	introText.setPosition(target.getView().getCenter());

	target.draw(introText);

	introText.setString("<Copyright placeholder>");
	introText.setCharacterSize(18U);

	{
		auto rect = introText.getLocalBounds();
		introText.setOrigin(rect.width / 2.f, rect.height / 2.f);
		introText.setPosition(target.getView().getCenter().x, target.getView().getSize().y - rect.height);
	}

	target.draw(introText);
}