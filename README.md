# LUDO
Ludo game the game with a great love/hate relationship.
Here you will find a ZMQ version where you pick one of the four colors and roll a digital dice.
Like in the boardgame version you got 4 pions and can eliminate your opponent pions and get yours to safety.

## Overview 

Two, three, or four can play, without partnerships.
At the beginning of the game,each player's four tokens are out of play and staged in the player's yard
(one of the large corner areas of the board in the player's colour).
When able to, the players enter their tokens one per turn on their respective starting squares and proceed to race them clockwise around the board along the game track (the path of squares not part of any player's home column).
When reaching the square below their home column, a player continues by moving tokens up the column to the finishing square.
The rolls of a single die control the swiftness of the tokens, and entry to the finishing square requires a precise roll from the player.
The first to bring all their tokens to the finish wins the game.
The others often continue to play to determine second-, third-, and fourth-place finishers. 

## Gameplay

The players will pick one of the four colors and in the same picking order will the turns rotate.
To enter a token into play from its yard to its starting square, a player must roll a six. Players can draw a token from home every time they get a six unless home is empty or move a piece six times

Players must always move a token according to the die value rolled.
Once players have one or more tokens in play, they select a token and move it forwards along the track the number of squares indicated by the die.
If you roll a certain number and that forces your token to land on a spot occupied by another person's token then you capture that token and force the person to return that token back to their home point.
This forces them to roll another 6 to take it out of their home and move it again

rolling a six earns the player an additional or "bonus" roll in that turn.
the extra roll if for the same token that is used for moving or bringing out of the yard.

After the token is in the home column that token is save and can not longer be playable.
The first one to bring all four tokens in the home column wins.

## Diagram

![diagram](./media/diagram.png?raw=true)

![example commands help and start](./media/DiagramStartHelp.png?raw=true)

![example game](./media/DiagramExample.png?raw=true)


## Commands 

The topic that is needed for the server is ``Ludo?>``
There is no need to add your color in any commands because the rotation is fixed

for all the specific commands are found below.  

For help menu and a list with the commands:

<pre><code><i>ludo?>help</code></pre>

The start the game:

<pre><code><i>ludo?>start</code></pre>

The pick how many players are playing (2 in the example, can pick 2-4):

<pre><code><i>ludo?>start>2</code></pre>

The rage quit and destroy the game:

<pre><code><i>ludo?>exit</code></pre>

The roll the dice for specific token:

<pre><code><i>ludo?>player>roll>1st token</code></pre>
<pre><code><i>ludo?>player>roll>2nd token</code></pre>
<pre><code><i>ludo?>player>roll>3rd token</code></pre>
<pre><code><i>ludo?>player>roll>4th token</code></pre>

To free a new token after rolling a 6:

<pre><code><i>ludo?>player>free token</code></pre>







