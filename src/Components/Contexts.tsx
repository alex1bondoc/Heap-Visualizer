// Contexts.js
import {createContext} from 'react';

export const FreeFunctionContext = createContext(null);
export const MallocFunctionContext = createContext((size: number) => {});
export const ReallocFunctionContext = createContext(null);
export const WasmInstanceContext = createContext(null);
