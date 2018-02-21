/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2015 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::parmetisDecomp

Description
    parmetis domain decomposition

SourceFiles
    parmetisDecomp.C

\*---------------------------------------------------------------------------*/
#ifndef parmetisDecomp_H
#define parmetisDecomp_H

#include "decompositionMethod.H"

namespace Foam
{

/*---------------------------------------------------------------------------*\
                        Class parmetisDecomp Declaration
\*---------------------------------------------------------------------------*/

class parmetisDecomp
:
    public decompositionMethod
{
    // Private data

  //     const polyMesh& mesh_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        parmetisDecomp(const parmetisDecomp&);

        //- Disallow default bitwise assignment
        void operator=(const parmetisDecomp&);


        //- Insert list in front of list
        template<class Type>
        static void prepend(const UList<Type>&, List<Type>&);

        //- Insert list at end of list
        template<class Type>
        static void append(const UList<Type>&, List<Type>&);

        label decompose
        (
            Field<int>& xadj,
            Field<int>& adjncy,
            const pointField& cellCentres,
            Field<int>& cellWeights,
            Field<int>& faceWeights,
            const List<int>& options,

            List<int>& finalDecomp
        );


public:

    //- Runtime type information
    TypeName("parmetis");


    // Constructors

        //- Construct given the decomposition dictionary and mesh
        parmetisDecomp(const dictionary&);


    // Destructor
       virtual ~parmetisDecomp()
        {}


    // Member Functions

        //- parmetis handles Foam processor boundaries
        virtual bool parallelAware() const
        {
            return true;
        }

       //- Inherit decompose from decompositionMethod
        using decompositionMethod::decompose;
        //- Return for every coordinate the wanted processor number. Use the
        //  mesh connectivity (if needed)
        //  Weights get normalised so the minimum value is 1 before truncation
        //  to an integer so the weights should be multiples of the minimum
        //  value. The overall sum of weights might otherwise overflow.
        virtual labelList decompose
        (
            const polyMesh& mesh,
            const pointField& points,
            const scalarField& pointWeights
        );

        //- Return for every coordinate the wanted processor number. Gets
        //  passed agglomeration map (from fine to coarse cells) and coarse cell
        //  location. Can be overridden by decomposers that provide this
        //  functionality natively.
        //  See note on weights above.
        virtual labelList decompose
        (
            const polyMesh& mesh,
            const labelList& cellToRegion,
            const pointField& regionPoints,
            const scalarField& regionWeights
        );

        //- Same but with uniform weights
        virtual labelList decompose
        (
            const polyMesh& mesh,
            const labelList& cellToRegion,
            const pointField& regionPoints
        )
        {
            return decompose
            (
                mesh,
                cellToRegion,
                regionPoints,
                scalarField(regionPoints.size(), 1.0)
            );
        }

        //- Return for every coordinate the wanted processor number. Explicitly
        //  provided mesh connectivity.
        //  The connectivity is equal to mesh.cellCells() except for
        //  - in parallel the cell numbers are global cell numbers (starting
        //    from 0 at processor0 and then incrementing all through the
        //    processors)
        //  - the connections are across coupled patches
        //  See note on weights above.
        virtual labelList decompose
        (
            const labelListList& globalCellCells,
            const pointField& cc,
            const scalarField& cWeights
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "parmetisDecompTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
