# Conway's Game of Life - Solution Summary

## Problem Overview
Implement Conway's Game of Life simulator with RLE (Run Length Encoded) input/output format.

## Final Score: 100/100

### Submission History
1. **Submission 767226**: 50/100 - TLE on pressure test 5 (22174ms)
2. **Submission 767240**: 50/100 - TLE on pressure test 5 (17100ms)  
3. **Submission 767250**: 100/100 - All tests passed (3498ms total) ✅

## Implementation Approach

### Data Structure
- Used `unordered_set<long long>` to store live cells (sparse grid representation)
- Encoded 2D coordinates (row, col) into 64-bit keys for efficient lookup
- This approach is memory efficient for large sparse grids (up to 13000x13000)

### Key Functions

#### 1. Initialize()
- Parse RLE format input with proper handling of:
  - `b` (dead cell), `o` (live cell)
  - `$` (newline), `!` (end marker)
  - Run-length encoding (e.g., `3o` = 3 consecutive live cells)

#### 2. Tick()
- **Optimization**: Use neighbor count map instead of nested loops
- For each live cell, increment neighbor count for its 8 adjacent cells
- Apply Conway's rules based on the neighbor counts
- Time complexity: O(m) where m is number of live cells

#### 3. PrintGame()
- **Optimization**: Only iterate through rows/columns that contain live cells
- Build a map of rows containing live cells, sort columns within each row
- Properly compress output with RLE format
- Handle consecutive live cells and empty row compression

#### 4. GetLiveCell()
- Simply return the size of the live_cells set: O(1)

## Performance Improvements

### Initial Implementation (TLE)
- Nested loops checking all neighbors: O(m × n) per tick
- Iterating through entire grid in PrintGame: O(rows × cols)
- Result: 22174ms, failed pressure test 5

### First Optimization (Still TLE)
- Changed to neighbor count map: reduced to 17100ms
- Still too slow due to PrintGame

### Final Optimization (Accepted)
- Optimized PrintGame to only process existing cells
- Result: 3498ms total, all tests passed
- Test 4: 1416ms (down from 8817ms → 5044ms)
- Test 5: 1981ms (down from 13003ms → 11848ms → 1981ms)

## Compression Quality
All test cases achieved compression ratio = 1.00 (same as reference solution)
- Received congratulations message: "Your compression ratio is almost the same as std :)"

## Key Insights
1. For sparse grids, hash-based data structures (unordered_set/map) are crucial
2. Avoid iterating through empty space in large grids
3. The PrintGame optimization was the critical bottleneck, not just Tick()
4. Proper RLE compression is important for scoring (50% of score comes from compression quality)
