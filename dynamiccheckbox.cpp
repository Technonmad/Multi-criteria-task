#include "dynamiccheckbox.h"

DynamicCheckBox::DynamicCheckBox(QWidget *parent) : QCheckBox(parent)
{
    res_id++;
    check_box_id = res_id;
}

DynamicCheckBox::~DynamicCheckBox(){}

int DynamicCheckBox::getID()
{
    return check_box_id;
}

void DynamicCheckBox::cleanID()
{
    res_id = 0;
}

int DynamicCheckBox::res_id = 0;
