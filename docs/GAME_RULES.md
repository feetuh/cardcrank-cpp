# Card Crank - Game Rules

## Overview
Card Crank is a shedding card game for 2-4 players. The objective is to empty your hand by playing cards onto a shared stack. Play continues even after a player empties their hand.

## Deck
- **Ranks:** 9, 10, J, Q, K, A (24 cards total)
- **Suits:** Hearts, Diamonds, Clubs, Spades (standard 4 suits)
- **Ordering:** 9 is lowest, A is highest (9 < 10 < J < Q < K < A)

## Setup
1. All 24 cards are dealt **evenly** to players in clockwise order
2. The player holding the **9 of Hearts (9♥)** starts the game

## Gameplay

### Starting the Game
- The player with 9♥ **must** play it alone as a single card to begin the stack
- This card becomes the permanent base of the stack

### Turn Structure
- Players take turns **clockwise**
- On each turn, a player may:
  1. **Play a valid card/combo** (see Valid Plays below)
  2. **Pass and pick up cards** (see Pickup Rules below)

### Valid Plays
A player may play card(s) if they meet **both** conditions:
1. **Rank condition:** The played card(s) must be of the **same rank or higher** than the top of the stack
2. **Count condition:** The number of cards played matches what's allowed (singles, or 4-of-a-kind)

#### Playing Singles
- Play **one card** of equal or higher rank than the stack's top card

#### Playing Four-of-a-Kind
- Play **all 4 cards of the same rank** (e.g., all four Kings)
- The rank must be **higher** than the top of the stack
- After playing 4-of-a-kind, the player **takes another turn immediately**
- This can chain: if you play another 4-of-a-kind on your bonus turn, you get another turn

#### Special 9♥ Rule
- If the stack contains **only the 9♥** and a player holds the **other three 9s**, they may play all three 9s at once
- This is treated **as if playing all four 9s** - the player gets another turn

### Pickup Rules
If a player **chooses not to play** or **cannot play** (no valid cards):

#### Option 1: Pickup 3 Cards
- Take the **top 3 cards** from the stack and add them to your hand
- If the stack has **fewer than 3 cards**, take whatever is available
- **If 9♥ is among the cards picked up, place it back as the base of the stack** (it stays at the bottom permanently)
- Your turn ends; you'll play again in the next round

#### Option 2: Take All Cards
- Take **all cards** from the stack and add them to your hand
- **MUST place the 9♥ back** as the base of the stack (it stays at the bottom permanently)
- Your turn ends; you'll play again in the next round

**Important:** The 9♥ **always** stays at the bottom of the stack. Whether you take 3 cards or all cards, if the 9♥ leaves the stack, it must be placed back as the base.

### Winning
- The goal is to **empty your hand**
- Play **continues** even after a player empties their hand (others can still win)
- The game ends when only one player has cards remaining (or when players agree to stop)

## Example Play
1. **Deal:** 4 players each get 6 cards. Player A has 9♥.
2. **Start:** Player A plays 9♥. Stack: `[9♥]`
3. **Player B:** Has 9♦, 10♣, K♥. Plays 10♣ (≥ last played 9♥). Stack: `[9♥, 10♣]`
4. **Player C:** Has no cards ≥ 10 (last played). Chooses to take all cards. Stack becomes `[9♥]`. Player C adds 10♣ to hand.
5. **Player D:** Has 9♠, 9♣, 9♦. Since stack is only 9♥, plays all three 9s (special rule!). Gets another turn. Stack: `[9♥, 9♠, 9♣, 9♦]`
6. **Player D (bonus):** Has four Kings. Plays all four Kings (≥ last played 9♦). Gets another turn!
7. **Player D (bonus):** Has no more 4-of-a-kinds. Plays Q♠ (single, ≥ last played K). Turn ends.
8. **Player A:** Continues clockwise...

## Summary of Key Mechanics
| Mechanic | Description |
|----------|-------------|
| Win condition | First to empty hand wins; play continues for others |
| Turn order | Always clockwise |
| 9♥ | Starts game, **always stays at bottom** |
| Valid plays | Singles OR 4-of-a-kind only |
| Rank comparison | Compare against most recently played card(s) |
| 4-of-a-kind | Play all 4, get bonus turn |
| 3× 9s on 9♥ | Treated as 4-of-a-kind, get bonus turn |
| Pickup | 3 cards (or fewer) OR all cards |
| 9♥ on pickup | **Always placed back as base** if removed |
| After pickup | Turn ends, play next round |

## Valid Plays (Clarified)
- **Singles:** Play one card of equal or higher rank than the last played card
- **Four-of-a-kind:** Play all four cards of the same rank (must be higher than last played)

**Note:** Only singles and four-of-a-kind plays are valid. Pairs, triples, and runs are NOT allowed.

## Winning (Clarified)
- The **first player to empty their hand** wins the game
- Play **continues** with remaining players until only one player has cards left (or players agree to stop)

## Stack Comparison
When determining if a play is valid, compare your card's rank against the **most recently played card(s)** on the stack. For example:
- Stack: `[9♥, 10♣, Q♠]` → Compare against Q♠ (need Q or higher)
- Stack: `[9♥, K♠, K♣, K♥, K♦]` → Compare against K♠ (need A, since A > K)
