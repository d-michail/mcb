//---------------------------------------------------------------------
// File automatically generated using notangle from DMIN_CYCLE_BASIS.lw
//
// emails and bugs: Dimitrios Michail <dimitrios.michail@gmail.com>
//---------------------------------------------------------------------
//
// This program can be freely used in an academic environment
// ONLY for research purposes, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    an acknowledgment in the product documentation is required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
// Any other use is strictly prohibited by the author, without an explicit 
// permission.
//
// Note that this program uses the LEDA library, which is NOT free. For more 
// details visit Algorithmic Solutions at http://www.algorithmic-solutions.com/
//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ! Any commercial use of this software is strictly !
// ! prohibited without explicit permission by the   !
// ! author.                                         !
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Copyright (C) 2004-2006 - Dimitrios Michail 


#include <iostream>

// Read a very simple format for graphs and write it as a GML graph.
int main() {
        int n,m;
        std::cin >> n;
        std::cin >> m;
        
        std::cout << "graph [" << std::endl;
        std::cout << "directed 0" << std::endl;
        std::cout << std::endl;
        
        for( int i = 0; i < n; ++i ) { 
                std::cout << "node [ id " << i << " ]" << std::endl;
        }

        std::cout << std::endl;
        
        int s,t,w;
        for( int i = 0; i < m; ++i ) { 
                std::cin >> s >> t >> w;
                std::cout << "edge [" << std::endl;
                std::cout << "\tsource " << s << std::endl;
                std::cout << "\ttarget " << t << std::endl;
                std::cout << "\tlabel " << w << std::endl;
                std::cout << "]" << std::endl;
        }
        std::cout << std::endl;
        std::cout << "]" << std::endl;
}

/* ex: set ts=8 sw=4 sts=4 noet: */


