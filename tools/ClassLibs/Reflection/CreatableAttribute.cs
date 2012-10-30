using System;
using System.Collections.Generic;
using System.Text;

namespace CoS
{
    [AttributeUsage(AttributeTargets.All)]
    public class CreatableAttribute : Attribute
    {
        private bool m_bCreatable;

        public CreatableAttribute(bool bCreatable)
        {
            m_bCreatable = bCreatable;
        }

        public virtual bool Creatable
        {
            get { return m_bCreatable; }
        }

        public override string ToString()
        {
            return m_bCreatable.ToString();
        }
    }
}
