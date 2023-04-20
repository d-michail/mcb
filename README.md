Minimum Cycle Basis (MCB) Library
---------------------------------

This page is the source repository for the package located at
<a href="https://d-michail.github.io/mcb/">d-michail.github.io/mcb</a>.

This package implements algorithms to compute exact and approximate 
Minimum Cycle Bases of weighted graphs.

A newer package containing a subset of these algorithms using the Boost Graph libraries and TBB, can 
be found at https://github.com/d-michail/parmcb.

For undirected graphs, four algorithms:

   - An O(m^3 + m n^2 \log n ) time exact algorithm
   - An O( m^2 n^2 ) time exact algorithm
   - An O( m^3 ) time exact algorithm
   - An O( m n^{1+1/k} + min(m^3 + m n^2 \log n, n^{3+3/k}) ) time 
     (2k-1)-approximate algorithm for any integer k >= 1

For directed graphs:

   - An O(m^3 + m n^2 \log n ) exact Monte-Carlo algorithm
   - An O( n^{3+3/k} \log n ) (2k-1)-approximate Monte-Carlo algorithm
   - An O( m n^{1+1/k} + min(m^3 + m n^2 \log n, n^{3+3/k}) ) time 
     (2k-1)-approximate Monte-Carlo algorithm for any integer k >= 1

This program can be freely used in an academic environment
ONLY for research purposes, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   an acknowledgment in the product documentation is required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Any other use is strictly prohibited by the author, without an explicit
permission.

Note that this program uses the LEDA library, which is NOT free. For more
details visit Algorithmic Solutions at http://www.algorithmic-solutions.com/

```
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! Any commercial use of this software is strictly !
! prohibited without explicit permission by the   !
! author.                                         !
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
```

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Copyright (C) 2004-2008 Dimitrios Michail

