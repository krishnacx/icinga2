/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012 Icinga Development Team (http://www.icinga.org/)        *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#include "base/array.h"
#include "base/objectlock.h"
#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/foreach.hpp>

using namespace icinga;

BOOST_AUTO_TEST_SUITE(base_array)

BOOST_AUTO_TEST_CASE(construct)
{
	Array::Ptr array = boost::make_shared<Array>();
	BOOST_CHECK(array);
	BOOST_CHECK(array->GetLength() == 0);
}

BOOST_AUTO_TEST_CASE(getset)
{
	Array::Ptr array = boost::make_shared<Array>();
	array->Add(7);
	array->Add(2);
	array->Add(5);
	BOOST_CHECK(array->GetLength() == 3);
	BOOST_CHECK(array->Get(0) == 7);
	BOOST_CHECK(array->Get(1) == 2);
	BOOST_CHECK(array->Get(2) == 5);

	array->Set(1, 9);
	BOOST_CHECK(array->Get(1) == 9);

	array->Remove(1);
	BOOST_CHECK(array->GetLength() == 2);
	BOOST_CHECK(array->Get(1) == 5);
}

BOOST_AUTO_TEST_CASE(remove)
{
	Array::Ptr array = boost::make_shared<Array>();
	array->Add(7);
	array->Add(2);
	array->Add(5);

	{
		ObjectLock olock(array);
		Array::Iterator it = array->Begin();
		array->Remove(it);
	}

	BOOST_CHECK(array->GetLength() == 2);
	BOOST_CHECK(array->Get(0) == 2);
}

BOOST_AUTO_TEST_CASE(foreach)
{
	Array::Ptr array = boost::make_shared<Array>();
	array->Add(7);
	array->Add(2);
	array->Add(5);

	ObjectLock olock(array);

	int n = 0;

	BOOST_FOREACH(const Value& item, array) {
		BOOST_CHECK(n != 0 || item == 7);
		BOOST_CHECK(n != 1 || item == 2);
		BOOST_CHECK(n != 2 || item == 5);

		n++;
	}
}

BOOST_AUTO_TEST_CASE(clone)
{
	Array::Ptr array = boost::make_shared<Array>();
	array->Add(7);
	array->Add(2);
	array->Add(5);

	Array::Ptr clone = array->ShallowClone();

	BOOST_CHECK(clone->GetLength() == 3);
	BOOST_CHECK(clone->Get(0) == 7);
	BOOST_CHECK(clone->Get(1) == 2);
	BOOST_CHECK(clone->Get(2) == 5);
}

BOOST_AUTO_TEST_CASE(seal)
{
	Array::Ptr array = boost::make_shared<Array>();
	array->Add(7);

	BOOST_CHECK(!array->IsSealed());
	array->Seal();
	BOOST_CHECK(array->IsSealed());

	BOOST_CHECK_THROW(array->Add(2), boost::exception);
	BOOST_CHECK(array->GetLength() == 1);

	BOOST_CHECK_THROW(array->Set(0, 8), boost::exception);
	BOOST_CHECK(array->Get(0) == 7);

	BOOST_CHECK_THROW(array->Remove(0), boost::exception);
	BOOST_CHECK(array->GetLength() == 1);
	BOOST_CHECK(array->Get(0) == 7);

	{
		ObjectLock olock(array);
		Array::Iterator it = array->Begin();
		BOOST_CHECK_THROW(array->Remove(it), boost::exception);
	}

	BOOST_CHECK(array->GetLength() == 1);
	BOOST_CHECK(array->Get(0) == 7);
}

BOOST_AUTO_TEST_CASE(serialize)
{
	Array::Ptr array = boost::make_shared<Array>();
	array->Add(7);
	array->Add(2);
	array->Add(5);

	String json = Value(array).Serialize();
	BOOST_CHECK(json.GetLength() > 0);

	Array::Ptr deserialized = Value::Deserialize(json);
	BOOST_CHECK(deserialized);
	BOOST_CHECK(deserialized->GetLength() == 3);
	BOOST_CHECK(deserialized->Get(0) == 7);
	BOOST_CHECK(deserialized->Get(1) == 2);
	BOOST_CHECK(deserialized->Get(2) == 5);
}

BOOST_AUTO_TEST_SUITE_END()
