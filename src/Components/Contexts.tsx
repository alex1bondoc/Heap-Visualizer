// Contexts.js
import {createContext} from 'react';

export const FreeFunctionContext = createContext((address: string) => {});
export const MallocFunctionContext = createContext((size: number) => {});
export const ReallocFunctionContext = createContext((address: string, size: number) => {});
export const WasmInstanceContext = createContext(null);
