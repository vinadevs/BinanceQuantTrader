import React, { useState, useMemo } from 'react'
import { Card, CardContent } from "@/components/ui/card"
import { Button } from "@/components/ui/button"
import { Input } from "@/components/ui/input"
import { Table, THead, TBody, Tr, Th, Td } from "./components/SimpleTable"
import { CSVLink } from 'react-csv'
import { BarChart, Bar, XAxis, YAxis, Tooltip, ResponsiveContainer } from 'recharts'
import { motion } from 'framer-motion'

// Single-file Trade Monitor React component
// - Drop-in React component (TailwindCSS + shadcn/ui assumed available)
// - Upload your log text file, parse balance and BinanceNewOrderAck entries
// - Display summary cards, orders table, simple charts, CSV export

export default function TradeMonitor() {
  const [raw, setRaw] = useState('')
  const [orders, setOrders] = useState([])
  const [balance, setBalance] = useState(null)
  const [filter, setFilter] = useState('')

  function handleFile(e) {
    const f = e.target.files?.[0]
    if (!f) return
    const reader = new FileReader()
    reader.onload = () => {
      const txt = String(reader.result || '')
      setRaw(txt)
      const { balance: b, orders: o } = parseLog(txt)
      setBalance(b)
      setOrders(o)
    }
    reader.readAsText(f)
  }

  function handlePaste() {
    navigator.clipboard.readText().then(text => {
      setRaw(text)
      const { balance: b, orders: o } = parseLog(text)
      setBalance(b)
      setOrders(o)
    }).catch(()=>{})
  }

  const filtered = useMemo(() => {
    if (!filter) return orders
    const q = filter.toLowerCase()
    return orders.filter(o => (
      (o.Symbol || '').toLowerCase().includes(q) ||
      (o.ClientOrderId || '').toLowerCase().includes(q) ||
      (o.OrderStatus || '').toLowerCase().includes(q)
    ))
  }, [orders, filter])

  const stats = useMemo(() => {
    const total = orders.length
    const bySide = orders.reduce((acc, o) => { acc[o.Side] = (acc[o.Side]||0)+1; return acc }, {})
    const filled = orders.filter(o => Number(o.FilledAmount) > 0)
    const pnlApprox = filled.reduce((acc,o)=>{
      // approximate pnl: if Side SELL assume we sold; we can't compute PnL without entry/exit pairs
      // but we can sum (FilledPrice * FilledAmount) for notional
      const val = Number(o.FilledPrice||0) * Number(o.FilledAmount||0)
      acc.notional += val
      return acc
    }, {notional:0})
    return { total, bySide, notional: pnlApprox.notional }
  }, [orders])

  return (
    <div className="p-6 max-w-7xl mx-auto">
      <motion.h1 initial={{y:-10,opacity:0}} animate={{y:0,opacity:1}} className="text-2xl font-semibold mb-4">Trade Monitor — Log Viewer</motion.h1>

      <div className="grid grid-cols-1 md:grid-cols-3 gap-4 mb-4">
        <Card>
          <CardContent>
            <div className="space-y-2">
              <div className="flex items-center gap-2">
                <input type="file" accept=".txt,.log" onChange={handleFile} />
                <Button onClick={handlePaste}>Paste from Clipboard</Button>
                <CSVLink
                  data={orders}
                  filename={`orders_export.csv`}
                  className="ml-auto inline-flex items-center px-3 py-1 rounded bg-slate-700 text-white"
                >Export CSV</CSVLink>
              </div>

              <div className="text-sm text-slate-600">Filter orders: <Input value={filter} onChange={e=>setFilter(e.target.value)} placeholder="symbol, clientId, status..." /></div>
              <div className="pt-2">
                <div className="text-xs text-slate-500">Summary</div>
                <div className="flex gap-3 mt-2">
                  <div className="p-2 bg-slate-50 rounded shadow-sm">
                    <div className="text-sm">Orders</div>
                    <div className="text-lg font-medium">{stats.total}</div>
                  </div>
                  <div className="p-2 bg-slate-50 rounded shadow-sm">
                    <div className="text-sm">Sides</div>
                    <div className="text-lg font-medium">{Object.entries(stats.bySide).map(([k,v])=> `${k}:${v}`).join(' ')}</div>
                  </div>
                  <div className="p-2 bg-slate-50 rounded shadow-sm">
                    <div className="text-sm">Notional (filled)</div>
                    <div className="text-lg font-medium">{Math.round(stats.notional)}</div>
                  </div>
                </div>
              </div>

              <div className="pt-3 text-sm text-slate-600">
                {balance ? (
                  <div>
                    <div className="font-medium">Balance (USDT)</div>
                    <div>WalletBalance: {balance.WalletBalance}</div>
                    <div>AvailableBalance: {balance.AvailableBalance}</div>
                    <div>UpdateTime: {balance.UpdateTime}</div>
                  </div>
                ) : (
                  <div>No balance parsed yet.</div>
                )}
              </div>
            </div>
          </CardContent>
        </Card>

        <Card className="md:col-span-2">
          <CardContent>
            <div style={{height:200}} className="w-full">
              <ResponsiveContainer width="100%" height="100%">
                <BarChart data={Object.entries(stats.bySide).map(([k,v])=>({side:k,count:v}))}>
                  <XAxis dataKey="side" />
                  <YAxis />
                  <Tooltip />
                  <Bar dataKey="count" />
                </BarChart>
              </ResponsiveContainer>
            </div>
          </CardContent>
        </Card>
      </div>

      <Card>
        <CardContent>
          <div className="overflow-auto">
            <table className="w-full table-auto text-sm">
              <thead className="bg-slate-100 sticky top-0">
                <tr>
                  <th className="p-2 text-left">Time</th>
                  <th className="p-2 text-left">Symbol</th>
                  <th className="p-2 text-left">Side</th>
                  <th className="p-2 text-left">Status</th>
                  <th className="p-2 text-left">Amount</th>
                  <th className="p-2 text-left">Filled</th>
                  <th className="p-2 text-left">LimitPrice</th>
                  <th className="p-2 text-left">FilledPrice</th>
                  <th className="p-2 text-left">ClientOrderId</th>
                </tr>
              </thead>
              <tbody>
                {filtered.map((o, idx) => (
                  <tr key={idx} className="border-b">
                    <td className="p-2">{o.UpdateTime}</td>
                    <td className="p-2">{o.Symbol}</td>
                    <td className="p-2">{o.Side}</td>
                    <td className="p-2">{o.OrderStatus}</td>
                    <td className="p-2">{o.Amount}</td>
                    <td className="p-2">{o.FilledAmount}</td>
                    <td className="p-2">{o.LimitPrice}</td>
                    <td className="p-2">{o.FilledPrice}</td>
                    <td className="p-2">{o.ClientOrderId}</td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        </CardContent>
      </Card>

      <div className="mt-4 text-xs text-slate-500">Raw file preview (first 1000 chars)</div>
      <pre className="mt-2 p-2 bg-slate-50 rounded max-h-40 overflow-auto text-xs">{raw.slice(0,1000)}</pre>
    </div>
  )
}

// -------------------------
// Parsing helpers (client side)
// -------------------------

function parseLog(text) {
  // parse balance block
  const balance = parseBalance(text)
  const orders = parseOrders(text)
  return { balance, orders }
}

function parseBalance(text) {
  // find block between "-- User Future Account Balance" and next ----
  const m = /--\s*User Future Account Balance([\s\S]*?)--/i.exec(text + '\n--')
  const out = {}
  if (!m) return null
  const block = m[1]
  // key: value pairs
  const lines = block.split(/\n/).map(l=>l.trim()).filter(Boolean)
  for (const line of lines) {
    const kv = line.split(':')
    if (kv.length>=2) {
      const k = kv[0].trim()
      const v = kv.slice(1).join(':').trim()
      out[k] = v
    }
  }
  // also try to parse asset line if present
  const assetRe = /Asset:\s*([A-Z0-9]+),\s*WalletBalance:\s*([^,]+)/i
  const am = assetRe.exec(text)
  if (am) {
    out['Asset'] = am[1]
    out['WalletBalance'] = am[2]
  }
  return out
}

function parseOrders(text) {
  // find all BinanceNewOrderAck(...) occurrences
  const re = /BinanceNewOrderAck\(([^)]+)\)/g
  const out = []
  let m
  while ((m = re.exec(text)) !== null) {
    const inside = m[1]
    const obj = parseKeyValuePairs(inside)
    out.push(obj)
  }
  return out
}

function parseKeyValuePairs(s) {
  // split by ',', but avoid commas inside values (there shouldn't be any). Simpler approach: split by ', ' and parse key: value
  const parts = s.split(/,\s*/)
  const obj = {}
  for (const part of parts) {
    const kv = part.split(':')
    if (kv.length>=2) {
      const k = kv[0].trim()
      const v = kv.slice(1).join(':').trim()
      obj[k] = v
    }
  }
  return obj
}
