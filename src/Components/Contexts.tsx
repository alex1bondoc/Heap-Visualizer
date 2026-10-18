// Contexts.js
import {createContext} from 'react';

export const FreeFunctionContext = createContext<(address: string) => void>(() => {});
export const MallocFunctionContext = createContext<(size: number) => void>(() => {});
export const ReallocFunctionContext = createContext<(address: string, size: number) => void>(() => {});
export const ResetFunctionContext = createContext<() => void>(() => {});
