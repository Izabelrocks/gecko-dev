/*
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 * 
 * The Original Code is Mozilla MathML Project.
 * 
 * The Initial Developer of the Original Code is The University Of 
 * Queensland.  Portions created by The University Of Queensland are
 * Copyright (C) 1999 The University Of Queensland.  All Rights Reserved.
 * 
 * Contributor(s): 
 *   Roger B. Sidje <rbs@maths.uq.edu.au>
 *   David J. Fiddes <D.J.Fiddes@hw.ac.uk>
 *   Shyjan Mahamud <mahamud@cs.cmu.edu> (added TeX rendering rules)
 */

#ifndef nsMathMLmmultiscriptsFrame_h___
#define nsMathMLmmultiscriptsFrame_h___

#include "nsCOMPtr.h"
#include "nsMathMLContainerFrame.h"

//
// <mmultiscripts> -- attach prescripts and tensor indices to a base 
//

class nsMathMLmmultiscriptsFrame : public nsMathMLContainerFrame {
public:
  friend nsresult NS_NewMathMLmmultiscriptsFrame(nsIPresShell* aPresShell, nsIFrame** aNewFrame);

  NS_IMETHOD
  Init(nsIPresContext*  aPresContext,
       nsIContent*      aContent,
       nsIFrame*        aParent,
       nsIStyleContext* aContext,
       nsIFrame*        aPrevInFlow);

  NS_IMETHOD
  Place(nsIPresContext*      aPresContext,
        nsIRenderingContext& aRenderingContext,
        PRBool               aPlaceOrigin,
        nsHTMLReflowMetrics& aDesiredSize);

  NS_IMETHOD
  SetInitialChildList(nsIPresContext* aPresContext,
                      nsIAtom*        aListName,
                      nsIFrame*       aChildList)
  {
    nsresult rv;
    rv = nsMathMLContainerFrame::SetInitialChildList(aPresContext, aListName, aChildList);
    UpdatePresentationDataFromChildAt(1, 1, PR_FALSE);
    // switch the style of the postscripts and prescripts
    InsertScriptLevelStyleContext(aPresContext);
    // check whether or not this is an embellished operator
    EmbellishOperator();
    return rv;
  }

protected:
  nsMathMLmmultiscriptsFrame();
  virtual ~nsMathMLmmultiscriptsFrame();
  
  virtual PRIntn GetSkipSides() const { return 0; }

private:
  nscoord mScriptSpace;  // scriptspace from TeX for extra spacing after sup/subscript
                         // = 0.5pt in plain TeX
  float   mSubScriptShiftFactor, mSupScriptShiftFactor;
  PRBool  mSubUserSetFlag, mSupUserSetFlag;
};

#endif /* nsMathMLmmultiscriptsFrame_h___ */
