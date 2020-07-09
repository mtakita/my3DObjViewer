#pragma once

#ifndef _CSELECTABLE_H_
#define _CSELECTABLE_H_

class CSelectable
{
	virtual void SelectActor() = 0;
	virtual void DeselectActor() = 0;
};

#endif
