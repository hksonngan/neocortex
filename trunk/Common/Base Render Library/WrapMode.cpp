/*
   Base Render Library   
   Copyright (C) 2009  Denis Bogolepov ( bogdencmc@inbox.ru )

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see http://www.gnu.org/licenses.
 */

#include "WrapMode.h"

namespace Render
{
        //-------------------------------------- Public Constants -------------------------------------
        
        const WrapMode WrapMode :: Clamp ( GL_CLAMP, GL_CLAMP, GL_CLAMP );
        
        const WrapMode WrapMode :: Repeat ( GL_REPEAT, GL_REPEAT, GL_REPEAT );

        //---------------------------------------- Constructor ----------------------------------------

        WrapMode :: WrapMode ( int s, int t, int r )
        {
                WrapS = s;

                WrapT = t;

                WrapR = r;
        }

        //----------------------------------------- Setup Mode ----------------------------------------
                        
        void WrapMode :: Setup ( int target )
        {
                glTexParameteri ( target, GL_TEXTURE_WRAP_S, WrapS );
                        
                glTexParameteri ( target, GL_TEXTURE_WRAP_T, WrapT );
                                
                glTexParameteri ( target, GL_TEXTURE_WRAP_R, WrapR );
        }
}
