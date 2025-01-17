/*
 * Copyright (C) 2020 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "AggregateErrorPrototype.h"
#include "ErrorType.h"
#include "InternalFunction.h"
#include "JSCJSValue.h"
#include "JSCell.h"
#include "JSGlobalObject.h"
#include "JSTypeInfo.h"
#include "Structure.h"
#include "VM.h"

namespace JSC {

class AggregateErrorConstructor final : public InternalFunction {
public:
    using Base = InternalFunction;

    DECLARE_INFO;

    static Structure* createStructure(VM& vm, JSGlobalObject* globalObject, JSValue prototype)
    {
        return Structure::create(vm, globalObject, prototype, TypeInfo(InternalFunctionType, StructureFlags), info());
    }

    static AggregateErrorConstructor* create(VM& vm, Structure* structure, AggregateErrorPrototype* prototype)
    {
        AggregateErrorConstructor* constructor = new (NotNull, allocateCell<AggregateErrorConstructor>(vm.heap)) AggregateErrorConstructor(vm, structure);
        constructor->finishCreation(vm, prototype);
        return constructor;
    }

    Structure* errorStructure(VM&) { return globalObject()->errorStructure(ErrorType::AggregateError); }

private:
    explicit AggregateErrorConstructor(VM&, Structure*);

    void finishCreation(VM&, AggregateErrorPrototype*);
};

} // namespace JSC
