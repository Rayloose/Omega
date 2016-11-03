#include "app.h"
#include "calculation_icon.h"

namespace Calculation {

App::App(Context * context) :
  ::App(&m_editExpressionController, "Calcul", ImageStore::CalculationIcon),
  m_globalContext(context),
  m_evaluateContext(EvaluateContext(context, &m_calculationStore)),
  m_calculationStore(CalculationStore()),
  m_historyController(HistoryController(&m_editExpressionController, &m_calculationStore)),
  m_editExpressionController(EditExpressionController(&m_modalViewController, &m_historyController, &m_calculationStore, this))
{
}

Context * App::globalContext() {
  return m_globalContext;
}

Context * App::evaluateContext() {
  return &m_evaluateContext;
}

}
